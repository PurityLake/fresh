#include "fresh/scope.h"

#include <stdio.h>
#include <string.h>

Error *create_function_ScopeEntry(ScopeEntry **se, char *name, FreshFunction func) {
	if (se == NULL) {
		return create_Error(NoObj, "NULL passed to create_function_ScopEntry: " __FILE__, __LINE__, 0);
	}
	*se = malloc(sizeof **se);
	if (*se == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_function_ScopeEntry: " __FILE__, __LINE__, 0);
	}
	(*se)->func = func;
	(*se)->type = SCOPE_FUNCTION;
	
	int len = strlen(name) + 1;
	(*se)->name = malloc(sizeof(char) * len);
	if ((*se)->name == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_function_ScopErntry: " __FILE__, __LINE__, 0);
	}
	strcpy((*se)->name, name);
	return NoError;
}
Error *create_sexp_ScopeEntry(ScopeEntry **se, char *name, Sexp *sexp) {
	if (se == NULL) {
		return create_Error(NoObj, "NULL passed to create_sexp_ScopEntry: " __FILE__, __LINE__, 0);
	}
	*se = malloc(sizeof **se);
	if (*se == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_sexp_ScopeEntry: " __FILE__, __LINE__, 0);
	}
	(*se)->sexp = sexp;
	(*se)->type = SCOPE_SEXP;

	int len = strlen(name) + 1;
	(*se)->name = malloc(sizeof(char) * len);
	if ((*se)->name == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_sexp_ScopErntry: " __FILE__, __LINE__, 0);
	}
	strcpy((*se)->name, name);
	return NoError;
}

Error *free_ScopeEntry(ScopeEntry **se) {
	if (se != NULL && *se != NULL) {
		if ((*se)->type == SCOPE_SEXP) {
			free_Sexp(&(*se)->sexp);
		}
		if ((*se)->name != NULL) {
			free((*se)->name);
		}
		free(*se);
	}
	return NoError;
}

BOOL is_function_ScopeEntry(const ScopeEntry *se) {
	return se->type == SCOPE_FUNCTION ? TRUE : FALSE;
}
BOOL is_sexp_ScopeEntry(const ScopeEntry *se) {
	return se->type == SCOPE_SEXP ? TRUE : FALSE;
}

Error *create_Scope(Scope **s, int capacity, Scope *parent) {
	if (s == NULL) {
		return create_Error(NoObj, "Passed NULL when creating a scope in create_Scope: " __FILE__, __LINE__, 0);
	}
	*s = malloc(sizeof **s);
	if (*s == NULL) {
		return create_Error(NoObj, "Failed to allocate memory for scope in create_Scope: " __FILE__, __LINE__, 0);
	}
	(*s)->entries = malloc(sizeof(ScopeEntry *) * capacity);
	if ((*s)->entries == NULL) {
		return create_Error(NoObj, "Failed to allocate memory for entries for scope in create_Scope: " __FILE__, __LINE__, 0);
	}
	(*s)->capacity = capacity;
	(*s)->size = 0;
	(*s)->parent = parent;
	return NoError;
}

Error *free_Scope(Scope **s) {
	if (s == NULL || *s == NULL) return NoError;
	for (int i = 0; i < (*s)->size; ++i) {
		free_ScopeEntry(&(*s)->entries[i]);
	}
	free((*s)->entries);
	free(*s);
	return NoError;
}

Error *add_to_Scope(Scope **s, ScopeEntry *entry) {
	if (s == NULL || *s == NULL) {
		return create_Error(NoObj, "Passed NULL when adding to scope in add_to_Scope: " __FILE__, __LINE__, 0);
	}
	if ((*s)->capacity <= (*s)->size + 1) {
		(*s)->capacity += 32;
		(*s)->entries = realloc((*s)->entries, sizeof(ScopeEntry *) * (*s)->capacity);
	}
	(*s)->entries[(*s)->size++] = entry;
	return NoError;
}
Error *get_from_Scope(Scope **s, char *name, ScopeEntry **out) {
	if (s == NULL || *s == NULL) {
		return create_Error(NoObj, "Passed NULL when trying to access member of scope in get_from_Scope: " __FILE__, __LINE__, 0);
	}
	for (int i = 0; i < (*s)->size; ++i) {
		if (strcmp(name, (*s)->entries[i]->name) == 0) {
			*out = (*s)->entries[i];
			return NoError;
		}
	}
	if ((*s)->parent != NULL) {
		return get_from_Scope(&(*s)->parent, name, out);
	}
	char *error = malloc(sizeof *s * 128);
	sprintf(error, "Couldn't find '%s' in scope", name);
	Error *e = create_Error(*s, error, 0, 0);
	free(error);
	return e;
}
