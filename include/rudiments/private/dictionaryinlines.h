// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/containerutilinlines.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARY_CLASS \
	dictionary<keytype,valuetype>

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::dictionary() {
	trackinsertionorder=true;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::~dictionary() {
	clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::setTrackInsertionOrder(bool trackinsertionorder) {
	if (!tree.getLength()) {
		this->trackinsertionorder=trackinsertionorder;
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getTrackInsertionOrder() {
	return trackinsertionorder;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::setValue(keytype key, valuetype value) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		dnode->setValue(value);
	} else {
		dnode=new dictionarynode<keytype,valuetype>(key,value);
		tree.insert(dnode);
		if (trackinsertionorder) {
			list.append(dnode);
		}
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getValue(keytype key, valuetype *value) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		*value=dnode->getValue();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype DICTIONARY_CLASS::getValue(keytype key) {
	valuetype	value;
	if (getValue(key,&value)) {
		return value;
	}
	return (valuetype)0;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
dictionarynode<keytype,valuetype> *DICTIONARY_CLASS::getNode(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		return tnode->getValue();
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::remove(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		return tree.remove(tnode);
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::clear() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlist<keytype> *DICTIONARY_CLASS::getKeys() {
	linkedlist<keytype>	*keys=new linkedlist<keytype>();
	for (linkedlistnode< dictionarynode< keytype, valuetype > *>
		*node=getList()->getFirst(); node; node=node->getNext()) {
		keys->append(node->getValue()->getKey());
	}
	return keys;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltree< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::getTree() {
	return &tree;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlist< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::getList() {
	if (!trackinsertionorder) {
		list.clear();
		for (avltreenode< dictionarynode< keytype, valuetype > *>
			*node=tree.getFirst(); node; node=node->getNext()) {
			list.append(node->getValue());
		}
	}
	return &list;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::print() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		node->getValue()->print();
		stdoutput.printf("\n");
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::
							find(keytype key) {
	dictionarynode<keytype,valuetype>	fnode(key,(valuetype)0);
	return tree.find(&fnode);
}

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,valuetype>

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::dictionarynode(keytype key, valuetype value) {
	this->key=key;
	this->value=value;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::~dictionarynode() {}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::setKey(keytype key) {
	this->key=key;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
keytype DICTIONARYNODE_CLASS::getKey() const {
	return key;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype DICTIONARYNODE_CLASS::getValue() const {
	return value;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t DICTIONARYNODE_CLASS::compare(keytype testkey) const {
	return _containerutil_compare(key,testkey);
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t DICTIONARYNODE_CLASS::compare(
		dictionarynode<keytype,valuetype> *testnode) const {
	return _containerutil_compare(key,testnode->key);
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::print() const {
	_containerutil_print(key);
	stdoutput.printf(":");
	_containerutil_print(value);
}
