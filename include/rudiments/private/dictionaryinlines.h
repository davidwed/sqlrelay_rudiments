// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

template <class keytype, class valuetype, class dictionarynodetype>
inline dictionary<keytype,valuetype,dictionarynodetype>::dictionary() {
}

template <class keytype, class valuetype, class dictionarynodetype>
inline dictionary<keytype,valuetype,dictionarynodetype>::~dictionary() {
	dict.clear();
}

template <class keytype, class valuetype, class dictionarynodetype>
inline void dictionary<keytype,valuetype,dictionarynodetype>::
				setValue(keytype key, valuetype value) {

	dictionarynode<keytype,valuetype>	*node;
	if (dict.getValueByKey(key,&node)) {
		node->setValue(value);
	} else {
		node=new dictionarynodetype(key,value);
		dict.append(node);
	}
}

template <class keytype, class valuetype, class dictionarynodetype>
inline int dictionary<keytype,valuetype,dictionarynodetype>::
				getValue(keytype key, valuetype *value) {
	dictionarynode<keytype,valuetype>	*node;
	if (dict.getValueByKey(key,&node)) {
		*value=node->getValue();
		return 1;
	}
	return 0;
}

template <class keytype, class valuetype, class dictionarynodetype>
inline int dictionary<keytype,valuetype,dictionarynodetype>::
						removeValue(keytype key) {
	return dict.removeByKey(key);
}

template <class keytype, class valuetype, class dictionarynodetype>
inline unsigned long dictionary<keytype,valuetype,dictionarynodetype>::
							getLength() const {
	return dict.getLength();
}

template <class keytype, class valuetype, class dictionarynodetype>
inline int dictionary<keytype,valuetype,dictionarynodetype>::
			getKey(unsigned long index, keytype *key) const {
	dictionarynode<keytype,valuetype>	*node;
	if (dict.getValueByIndex(index,&node)) {
		*key=node->getKey();
		return 1;
	}
	return 0;
}
