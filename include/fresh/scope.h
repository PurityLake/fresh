#include <stdlib.h>

#include "defines.h"
#include "error.h"
#include "sexp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef Error* (*FreshFunction)(Sexp*, Sexp**);

enum ScopeEntryType {
	SCOPE_SEXP,
	SCOPE_FUNCTION
};

typedef struct {
	char *name;
	enum ScopeEntryType type;
	union {
		Sexp *sexp;
		FreshFunction func;
	};
} ScopeEntry;

extern Error *create_function_ScopeEntry(ScopeEntry **se, char *name, FreshFunction func);
extern Error *create_sexp_ScopeEntry(ScopeEntry **se, char *name, Sexp *sexp);

extern Error *free_ScopeEntry(ScopeEntry **se);

extern BOOL is_function_ScopeEntry(const ScopeEntry *se);
extern BOOL is_sexp_ScopeEntry(const ScopeEntry *se);

typedef struct Scope {
	int size;
	int capacity;
	struct Scope *parent;
	ScopeEntry **entries
} Scope;

#ifdef __cplusplus
}
#endif
