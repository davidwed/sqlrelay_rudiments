// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

template <class keytype, class datatype, class dictionarynodetype>
inline dictionary<keytype,datatype,dictionarynodetype>::dictionary() {
}

template <class keytype, class datatype, class dictionarynodetype>
inline dictionary<keytype,datatype,dictionarynodetype>::~dictionary() {
	dict.clear();
}

template <class keytype, class datatype, class dictionarynodetype>
inline void dictionary<keytype,datatype,dictionarynodetype>::
				setData(keytype key, datatype data) {

	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByKey(key,&node)) {
		node->setData(data);
	} else {
		node=new dictionarynodetype(key,data);
		dict.append(node);
	}
}

template <class keytype, class datatype, class dictionarynodetype>
inline int dictionary<keytype,datatype,dictionarynodetype>::
				getData(keytype key, datatype *data) {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByKey(key,&node)) {
		*data=node->getData();
		return 1;
	}
	return 0;
}

template <class keytype, class datatype, class dictionarynodetype>
inline int dictionary<keytype,datatype,dictionarynodetype>::
						removeData(keytype key) {
	return dict.removeByKey(key);
}

template <class keytype, class datatype, class dictionarynodetype>
inline unsigned long dictionary<keytype,datatype,dictionarynodetype>::
							getLength() const {
	return dict.getLength();
}

template <class keytype, class datatype, class dictionarynodetype>
inline int dictionary<keytype,datatype,dictionarynodetype>::
			getKey(unsigned long index, keytype *key) const {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByIndex(index,&node)) {
		*key=node->getKey();
		return 1;
	}
	return 0;
}

template <class keytype, class datatype, class dictionarynodetype>
inline int dictionary<keytype,datatype,dictionarynodetype>::
			getData(unsigned long index, datatype *data) const {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByIndex(index,&node)) {
		*data=node->getData();
		return 1;
	}
	return 0;
}
