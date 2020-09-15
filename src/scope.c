#include "fresh/scope.h"

#include <string.h>

Error *create_function_ScopeEntry(ScopeEntry **se, char *name, FreshFunction func) {
	if (se == NULL) {
		return create_Error(NoObj, "NULL passed to create_function_ScopEntry: " __FILE__, __LINE__, 0);
	}
	*se = malloc(sizeof(ScopeEntry));
	if (*se == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_function_ScopeEntry: " __FILE__, __LINE__, 0);
	}
	(*se)->func = func;
	(*se)->type = SCOPE_FUNCTION;
	
	int len = strlen(name);
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
	*se = malloc(sizeof(ScopeEntry));
	if (*se == NULL) {
		return create_Error(NoObj, "Failed to allocate memory in create_sexp_ScopeEntry: " __FILE__, __LINE__, 0);
	}
	(*se)->sexp = sexp;
	(*se)->type = SCOPE_SEXP;

	int len = strlen(name);
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