// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#define DONT_INLINE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/dictionary.h>

// explicitly intstantiate namevaluepairs templates
template class stringdictionarynode< char * >;
template class stringdictionarylistnode< char * >;
template class stringdictionarylist< char * >;
template class stringdictionary< char * >;

template class dictionarynode< char *, char * >;
template class dictionarylistnode< char *, char * >;
template class dictionarylist< char *, char * >;
template class dictionary< char *, char *,
			stringdictionarynode< char * >,
			stringdictionarylistnode< char * >,
			stringdictionarylist< char * > >;

template class linkedlistnode< dictionarynode< char *, char * > * >;
template class linkedlist< dictionarynode< char *, char * > *,
				dictionarylistnode< char *, char* > >;
