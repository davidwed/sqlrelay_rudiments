// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOMNODE_H
#define RUDIMENTS_XMLDOMNODE_H

#include <rudiments/private/xmldomnodeincludes.h>

enum xmldomnodetype {
	NULL_XMLDOMNODETYPE=0,
	ROOT_XMLDOMNODETYPE,
	TAG_XMLDOMNODETYPE,
	ATTRIBUTE_XMLDOMNODETYPE,
	TEXT_XMLDOMNODETYPE,
	COMMENT_XMLDOMNODETYPE,
	CDATA_XMLDOMNODETYPE
};

class xmldom;
class xmldomnodeprivate;

/** The xmldomnode class provides a generic container for DOM tree elements.
 *  One can navigate the nodes of the tree, modify the tree and read or modify
 *  the data that the nodes contain by calling methods in this class.
 * 
 *  A DOM tree node can be one of the following:
 *  	the document root
 *  	a tag 
 * 	a tag attribute
 * 	a segment of text
 * 	a comment
 * 	a segment of CDATA
 * 
 *  Each node may contain the following data:
 *  (For some node types, the data container is unused and set to NULL)
 *  	type
 *  	namespace
 *  	name
 *  	value
 *  	parent node
 *  	next sibling
 *  	previous sibling
 *  	a list of attribute nodes
 *  	a list of child nodes
 *  	
 *  Here is a breakdown by node type:
 * 
 *  	For the document root:
 *  		type - ROOT_XMLDOMNODETYPE
 *  		namespace - unused
 *  		name - "document"
 *  		value - unused
 *  		parent node - unused
 *  		next sibling - unused
 *  		previous sibling - unused
 *  		a list of attribute nodes - unused
 *  		a list of child nodes -	the xml version tag, the doctype tag
 *  					and the top-level enclosing tag
 * 
 *  	For a tag:
 *  		type - TAG_XMLDOMNODETYPE
 *  		namespace - the tag namespace
 *  		name - the tag name
 *  		value - unused
 *  		parent node - the parent tag or document root
 *  		next sibling - can be another tag, a segment of text,
 *  					 a comment or a segment of cdata
 *  		previous sibling - can be another tag, a segment of text,
 *  					 a comment or a segment of cdata
 *  		a list of attribute nodes - a list of attribute nodes
 *  		a list of child nodes - a list of tags, text segments, comments
 *  					and/or cdata segments
 * 
 *  	For a tag attribute:
 *  		type - ATTRIBUTE_XMLDOMNODETYPE
 *  		namespace - unused
 *  		name - the attribute name
 *  		value - the attribute value
 *  		    (note that for tags with standalone
 *  		    attributes, the name and value are the same)
 *  		parent node - the tag containing the attributes
 *  		next sibling - the next attribute
 *  		previous sibling - the previous attribute
 *  		a list of attribute nodes - unused
 *  		a list of child nodes - unused
 * 
 *  	For a segment of text:
 *  		type - TEXT_XMLDOMNODETYPE
 *  		namespace - unused
 *  		name - "text"
 *  		value - the text itself
 *  		parent node - the tag containing the text
 *  		next sibling - can be a tag, a comment or a segment of cdata
 *  		previous sibling - can be a tag, a comment or a segment of cdata
 *  		a list of attribute nodes - unused
 *  		a list of child nodes - unused
 * 
 *  	For a comment:
 *  		type - COMMENT_XMLDOMNODETYPE
 *  		namespace - unused
 *  		name - "comment"
 *  		value - the comment itself
 *  		parent node - the tag containing the comment
 *  		next sibling - can be a tag, a segment of text, another
 *  				comment or a segment of cdata
 *  		previous sibling - can be a tag, a segment of text, another
 *  				comment or a segment of cdata
 *  		a list of attribute nodes - unused
 *  		a list of child nodes - unused
 * 
 *  	For a segment of cdata:
 *  		type - CDATA_XMLDOMNODETYPE
 *  		namespace - unused
 *  		name - "cdata"
 *  		value - the cdata itself
 *  		parent node - the tag containing the cdata
 *  		next sibling - can be a tag, a segment of text, a comment
 *  				or another segment of cdata
 *  		previous sibling - can be a tag, a segment of text, a comment
 *  				or another segment of cdata
 *  		a list of attribute nodes - unused
 *  		a list of child nodes - unused */
class RUDIMENTS_DLLSPEC xmldomnode {
	public:
			/** Creates a new node and intializes its
			 *  member variables to NULL.
			 * 
			 *  Your application should pass in a special
			 *  "nullnode" which may be created by the
			 *  static method createNullNode() below.
			 * 
			 *  This will keep command chaining like this:
			 * 
			 *  mynode->getFirstChild("node1")->
			 *  	getFirstChild("node2")->getName("node3");
			 * 
			 *  from causing the program to crash trying to
			 *  dereference a NULL pointer if, for example,
			 *  "node2" doesn't exist. */
			xmldomnode(xmldom *dom, xmldomnode *nullnode);

			/** Creates a new node (see
 			 *  xmldomnode(xmldom *, xmldomnode *) and intializes
 			 *  its member variables to the values passed in. */
			xmldomnode(xmldom *dom,
					xmldomnode *nullnode,
					xmldomnodetype type,
					const char *name,
					const char *value);

			/** Creates a new node (see
 			 *  xmldomnode(xmldom *, xmldomnode *) and intializes
 			 *  its member variables to the values passed in. */
			xmldomnode(xmldom *dom,
					xmldomnode *nullnode,
					xmldomnodetype type,
					const char *ns,
					const char *name,
					const char *value);

			/** Deletes the node, all attribute nodes and
			 *  optionally all child nodes, recursively. */
			~xmldomnode();


		/** Creates a special "null node" whose parent, next sibling,
		 *  previous sibling, and child point back to itself.  This
		 *  special node should be passed in when creating new
		 *  xmldomnodes.
		 * 
		 *  This method allocates xmldomnode internally and passes a
		 *  pointer back.  The calling program must ultimately
		 *  deallocate the node. */
		static	xmldomnode	*createNullNode(xmldom *dom);


		/** Instructs the destructor to recursively delete all child
		 *  nodes.  This is the default behavior. */
		void		cascadeOnDelete();

		/** Instructs the destructor not to recursively delete all
		 *  child nodes.  The default behavior is to delete all child
		 *  nodes. */
		void		dontCascadeOnDelete();


		/** Returns the type of the node. */
		xmldomnodetype	getType() const;

		/** Returns the namespace of the node. */
		const char	*getNamespace() const;

		/** Returns the name of the node. */
		const char	*getName() const;

		/** Returns the value of node. */
		const char	*getValue() const;


		/** Returns a pointer to the tree that this node is
		 *  attached to or NULL if it isn't attached to any tree. */
		xmldom		*getTree() const;

		/** Returns a pointer to the parent node or the
		 *  nullnode if none exists. */
		xmldomnode	*getParent() const;

		/** Returns the position of the node, relative to its
		 *  siblings. */
		uint64_t	getPosition() const;

		/** Returns a pointer to the previous sibling
		 *  node or the nullnode if none exists. */
		xmldomnode	*getPreviousSibling() const;

		/** Returns a pointer to the previous sibling
		 *  node whose type is TAG_XMLDOMNODE.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getPreviousTagSibling() const;

		/** Returns the previous sibling node named
		 *  "name" whose type is TAG_XMLDOMNODE or the
		 *  nullnode if not found. */
		xmldomnode	*getPreviousTagSibling(const char *name) const;

		/** Returns the previous sibling node in namespace
		 *  "ns" named "name" whose type is TAG_XMLDOMNODE or the
		 *  nullnode if not found. */
		xmldomnode	*getPreviousTagSibling(
						const char *ns,
						const char *name) const;

		/** Returns the previous sibling node named "name"
		 *  (ignoring case) whose type is TAG_XMLDOMNODE or the
		 *  nullnode if not found. */
		xmldomnode	*getPreviousTagSiblingIgnoringCase(
						const char *name) const;

		/** Returns the previous sibling node in namespace
		 *  "napesp" named "name" (both ignoring case) whose type is
		 *  TAG_XMLDOMNODE or the nullnode if not found. */
		xmldomnode	*getPreviousTagSiblingIgnoringCase(
						const char *ns,
						const char *name) const;

		/** Returns the previous sibling node named
		 *  "name" with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  whose type is TAG_XMLDOMNODE.  If "name" is
		 *  null, then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getPreviousTagSibling(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the previous sibling node in namespace
		 *  "ns" named "name" with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  whose type is TAG_XMLDOMNODE.  If "name" is
		 *  null, then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the previous sibling node named
		 *  "name" (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		xmldomnode	*getPreviousTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the previous sibling node in namespace
		 *  "ns" (ignoring case) named "name" (ignoring case)
		 *  with an attribute named "attributename" with value
		 *  "attributevalue" (both ignoring case) whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null, then the name of
		 *  the child node is not checked, and the first child
		 *  node with any name (with matching attribute name/value)
		 *  will be returned.  If no match is found, nullnode is
		 *  returned. */
		xmldomnode	*getPreviousTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns a pointer to the next sibling node
		 *  or the nullnode if none exists. */
		xmldomnode	*getNextSibling() const;

		/** Returns a pointer to the next sibling node
		 *  whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getNextTagSibling() const;

		/** Returns the next sibling node named "name"
		 *  whose type is TAG_XMLDOMNODE or the nullnode
		 *  if not found. */
		xmldomnode	*getNextTagSibling(const char *name) const;

		/** Returns the next sibling node in namespace "ns"
		 *  named "name" whose type is TAG_XMLDOMNODE or the nullnode
		 *  if not found. */
		xmldomnode	*getNextTagSibling(const char *ns,
							const char *name) const;

		/** Returns the next sibling node named "name"
		 *  (ignoring case) whose type is TAG_XMLDOMNODE
		 *  or the nullnode if not found. */
		xmldomnode	*getNextTagSiblingIgnoringCase(
						const char *name) const;

		/** Returns the next sibling node in namespace "ns"
		 *  named "name" (both ignoring case) whose type is
		 *  TAG_XMLDOMNODE or the nullnode if not found. */
		xmldomnode	*getNextTagSiblingIgnoringCase(
						const char *ns,
						const char *name) const;

		/** Returns the next sibling node named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getNextTagSibling(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the next sibling node in namespace
		 *  "ns" named "name" with an attribute named
		 *  "attributename" with value "attributevalue" whose
		 *  type is TAG_XMLDOMNODE.  If "name" is null, then the
		 *  name of the child node is not checked, and the first
		 *  child node with any name (with matching attribute
		 *  name/value) will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the next sibling node named "name"
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		xmldomnode	*getNextTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the next sibling node in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node is not
		 *  checked, and the first child node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getNextTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the next sibling node whose type is TAG_XMLDOMNODE
		 *  and whose name matches one of the members of the
		 *  NULL-terminated array "set".  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getNextTagSiblingInSet(
					const char * const *set) const;

		/** Returns the next sibling node in namespace "ns"
		 *  whose name matches one of the members of the
		 *  NULL-terminated array "set" and whose type is
		 *  TAG_XMLDOMNODE.  If no match is found, nullnode is
		 *  returned. */
		xmldomnode	*getNextTagSiblingInSet(
					const char *ns,
					const char * const *set) const;


		/** Returns the number of immediate child nodes. */
		uint64_t	getChildCount() const;

		/** Returns the firest child node named "name"
		 *  or the nullnode if not found. */
		xmldomnode	*getFirstChild(const char *name) const;

		/** Returns the firest child node in namespace "ns"
		 *  named "name" or the nullnode if not found. */
		xmldomnode	*getFirstChild(const char *ns,
						const char *name) const;

		/** Returns the child node named "name" (ignoring case)
		 *  or the nullnode if not found. */
		xmldomnode	*getFirstChildIgnoringCase(
						const char *name) const;

		/** Returns the child node in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case)
		 *  or the nullnode if not found. */
		xmldomnode	*getFirstChildIgnoringCase(
						const char *ns,
						const char *name) const;

		/** Returns the child node at index "position"
		 *  or the nullnode if not found. */
		xmldomnode	*getChild(uint64_t position) const;

		/** Returns the first child node named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue".  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstChild(const char *name,
						const char *attributename,
						const char *attributevalue)
						const;

		/** Returns the first child node in namespace "ns"
		 *  named "name" with an attribute named "attributename" with
		 *  value "attributevalue".  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstChild(const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue)
						const;

		/** Returns the first child node named "name"
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case).  If "name" is null, then
		 *  the name of the child node is not checked, and
		 *  the first child node with any name (with matching
		 *  attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getFirstChildIgnoringCase(
						const char *name,
						const char *attributename,
						const char *attributevalue)
						const;

		/** Returns the first child node in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case).  If "name" is null, then the name of
		 *  the child node is not checked, and the first child node
		 *  with any name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		xmldomnode	*getFirstChildIgnoringCase(
						const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue)
						const;

		/** Returns the first child node whose type is
		 *  TAG_XMLDOMNODE.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstTagChild() const;

		/** Returns the first child node named "name"
		 *  whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagChild(const char *name) const;

		/** Returns the first child node in namespace "ns"
		 *  named "name" whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagChild(const char *ns,
						const char *name) const;

		/** Returns the first child node named "name" (ignoring case)
		 *  whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagChildIgnoringCase(
						const char *name) const;

		/** Returns the first child node in namespace "ns"
		 *  named "name" (both ignoring case) whose type is
		 *  TAG_XMLDOMNODE.  If no match is found, nullnode is
		 *  returned. */
		xmldomnode	*getFirstTagChildIgnoringCase(
						const char *ns,
						const char *name) const;

		/** Returns the first child node named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstTagChild(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first child node in namespace "ns"
		 *  named "name" with an attribute named "attributename"
		 *  with value "attributevalue" whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node is not
		 *  checked, and the first child node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getFirstTagChild(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first child node named "name" 
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		xmldomnode	*getFirstTagChildIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first child node in namespace "ns"
		 *  (ignoring case) named "name" *  (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.  If
		 *  "name" is null, then the name of the child node is not
		 *  checked, and the first child node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getFirstTagChildIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first child node whose type is TAG_XMLDOMNODE
		 *  and whose name matches one of the members of the
		 *  NULL-terminated array "set".  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstTagChildInSet(
					const char * const *set) const;

		/** Returns the first child node in namespace "ns"
		 *  whose name matches one of the members of the
		 *  NULL-terminated array "set" and whose type is
		 *  TAG_XMLDOMNODE.  If no match is found, nullnode is
		 *  returned. */
		xmldomnode	*getFirstTagChildInSet(
					const char *ns,
					const char * const *set) const;


		/** Returns the first descendent node named "name"
		 *  whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendent(const char *name) const;


		/** Returns the first descendent node in namespace "ns"
		 *  named "name" whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendent(
						const char *ns,
						const char *name) const;


		/** Returns the first descendent node named "name"
		 *  (ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If no match is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendentIgnoringCase(
							const char *name) const;


		/** Returns the first descendent node in namespace "ns"
		 *  named "name" (both ignoring case) whose type is
		 *  TAG_XMLDOMNODE.  If no match is found, nullnode is
		 *  returned. */
		xmldomnode	*getFirstTagDescendentIgnoringCase(
						const char *ns,
						const char *name) const;

		/** Returns the first descendent node named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the descendent node is not
		 *  checked, and the first descendent node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstTagDescendent(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first descendent node in namespace "ns"
		 *  named "name" with an attribute named "attributename" with
		 *  value "attributevalue" whose type is TAG_XMLDOMNODE.  If
		 *  "name" is null, then the name of the descendent node is not
		 *  checked, and the first descendent node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first descendent node named "name" 
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the descendent node
		 *  is not checked, and the first descendent node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendentIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first descendent node in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.  If
		 *  "name" is null, then the name of the descendent node is not
		 *  checked, and the first descendent node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendentIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const;

		/** Returns the first descendent node whose type is
		 *  TAG_XMLDOMNODE and whose name matches one of the members of
		 *  the NULL-terminated array "set".  If no match is found,
		 *  nullnode is returned. */
		xmldomnode	*getFirstTagDescendentInSet(
					const char * const *set) const;

		/** Returns the first descendent node in namespace "ns"
		 *  whose name matches one of the members of the NULL-terminated
		 *  array "set" and whose type is TAG_XMLDOMNODE.  If no match
		 *  is found, nullnode is returned. */
		xmldomnode	*getFirstTagDescendentInSet(
					const char *ns,
					const char * const *set) const;

		/** Returns the next node in a depth-first traversal whose
		 *  type is TAG_XMLDOMNODE. */
		xmldomnode	*getNextTag() const;

		/** Returns the next node in a depth-first traversal of the
		 *  nodes beneath "top" whose type is TAG_XMLDOMNODE.
		 *
		 *  The behavior is undefined if "top" is not a direct
		 *  ancestor of this node. */
		xmldomnode	*getNextTag(xmldomnode *top) const;

		/** Returns the previous node in a depth-first traversal whose
		 *  type is TAG_XMLDOMNODE. */
		xmldomnode	*getPreviousTag() const;

		/** Returns the previous node in a depth-first traversal of the
		 *  nodes beneath "top" whose type is TAG_XMLDOMNODE.
		 *
		 *  The behavior is undefined if "top" is not a direct
		 *  ancestor of this node. */
		xmldomnode	*getPreviousTag(xmldomnode *top) const;


		/** Returns the number of attributes. */
		uint64_t	getAttributeCount() const;

		/** Returns the attribute named "name"
		 *  or the nullnode if not found. */
		xmldomnode	*getAttribute(const char *name) const;

		/** Returns the attribute named "name" (ignoring case)
		 *  or the nullnode if not found. */
		xmldomnode	*getAttributeIgnoringCase(
						const char *name) const;

		/** Returns the attribute node at index
		 *  "position" or the nullnode if not found. */
		xmldomnode	*getAttribute(uint64_t position) const;

		/** Returns the value of the attribute named
		 *  "name" or the nullnode if not found. */
		const char	*getAttributeValue(const char *name) const;

		/** Returns the value of the attribute node at
		 *  index "position" or the nullnode if not
		 *  found. */
		const char	*getAttributeValue(uint64_t position) const;

		/** Returns the attribute names and values in
		 *  a constnamevaluepairs dictionary.  The
		 *  instance of constnamevaluepairs is allocated
		 *  internally and must be deleted by the
		 *  calling program.  Returns NULL if the node
		 *  is a nullNode and an empty dictionary if the
		 *  node has no attributes. */
		constnamevaluepairs	*getAttributes() const;

		/** Sets the value of the attribute named
		 *  "name" to "value".  Creates attribute
		 *  "name" if it didn't previously exist. */
		void		setAttributeValue(const char *name,
							const char *value);

		/** Sets the value of the attribute named
		 *  "name" to "value".  Creates attribute
		 *  "name" if it didn't previously exist. */
		void		setAttributeValue(const char *name,
							int64_t value);

		/** Sets the value of the attribute named
		 *  "name" to "value".  Creates attribute
		 *  "name" if it didn't previously exist. */
		void		setAttributeValue(const char *name,
							uint64_t value);

		/** Returns the nullnode used by this node. */
		xmldomnode	*getNullNode() const;

		/** Returns true if this node is the special
		 *  nullnode and false otherwise. */
		bool		isNullNode() const;


		/** Sets the node type to "type". */
		void	setType(xmldomnodetype type);

		/** Sets the node namespace to "ns". */
		void	setNamespace(const char *ns);

		/** Sets the node name to "name". */
		void	setName(const char *name);

		/** Sets the node namespace to "ns" and node name to "name". */
		void	setName(const char *ns, const char *name);

		/** Sets the node value to "value". */
		void	setValue(const char *value);

		/** Sets the parent of the node to "parent". */
		void	setParent(xmldomnode *parent);

		/** Sets the previous sibling of the node to "previous". */
		void	setPreviousSibling(xmldomnode *previous);

		/** Sets the next sibling of the node to "next". */
		void	setNextSibling(xmldomnode *next);

		/** Inserts "child" into the list of child nodes at
		 *  "position".  The position of the next sibling
		 *  (and all successive siblings) is incremented. */
		bool	insertChild(xmldomnode *child, uint64_t position);

		/** Appends "child" to the list of child nodes. */
		bool	appendChild(xmldomnode *child);

		/** Inserts a new node of type TAG_XMLDOMNODETYPE with name
		 *  "tag" into the list of child nodes at "position".  The
		 *  position of the next sibling (and all successive siblings)
		 *  is incremented.
		 *
		 *  Returns the new node on success or the null node on
		 *  failure. */
		xmldomnode	*insertTag(const char *tag, uint64_t position);

		/** Inserts a new node of type TAG_XMLDOMNODETYPE with
		 *  namespace "ns" and name "tag" into the list of child
		 *  nodes at "position".  The position of the next sibling (and
		 *  all successive siblings) is incremented.
		 *
		 *  Returns the new node on success or the null node on
		 *  failure. */
		xmldomnode	*insertTag(const char *ns,
						const char *tag,
						uint64_t position);

		/** Parses "xml" and inserts the result into the list of child
		 *  nodes at "position".  The position of the next sibling (and
		 *  all successive siblings) is incremented as appropriate.
		 *
		 *  Returns true on success or false on failure. */
		bool	insertXml(const char *xml, uint64_t position);

		/** Parses file "xmlfile" and inserts the result into the list
		 *  of child nodes at "position".  The position of the next
		 *  sibling (and all successive siblings) is incremented as
		 *  appropriate.
		 *
		 *  Returns true on success or false on failure. */
		bool	insertXmlFile(const char *xmlfile, uint64_t position);

		/** Appends a new node of type TAG_XMLDOMNODETYPE with name
		 *  "tag" to the list of child nodes.
		 *
		 *  Returns the new node on success or the null node on
		 *  failure. */
		xmldomnode	*appendTag(const char *tag);

		/** Appends a new node of type TAG_XMLDOMNODETYPE with namespace
		 * "ns" and name "tag" to the list of child nodes.
		 *
		 *  Returns the new node on success or the null node on
		 *  failure. */
		xmldomnode	*appendTag(const char *ns, const char *tag);

		/** Parses "xml" and appends the result to the list of child
		 *  nodes.
		 *
		 *  Returns true on success or false on failure. */
		bool	appendXml(const char *xml);

		/** Parses file "xml" and appends the result to the list of
		 *  child nodes.
		 *
		 *  Returns true on success or false on failure. */
		bool	appendXmlFile(const char *xmlfile);

		/** Moves "child" from its current position into the
		 *  list of child nodes for "parent" at "position".
		 *
		 *  The behavior is undefined if "child" is not actually a
		 *  child of this node. */
		bool	moveChild(xmldomnode *child,
					xmldomnode *parent, uint64_t position);

		/** Deletes the child node at "position".  The position
		 *  of the next sibling (and all successive siblings)
		 *  is decremented. */
		bool	deleteChild(uint64_t position);

		/** Deletes child node "child".  The position of the next
		 *  sibling (and all successive siblings) is decremented.
		 *
		 *  The behavior is undefined if "child" is not actually a
		 *  child of this node. */
		bool	deleteChild(xmldomnode *child);

		/** Deletes the first child named "name".  The position
		 *  of the next sibling (and all successive siblings)
		 *  is decremented. */
		bool	deleteFirstChild(const char *name);

		/** Deletes the first child in namespace "ns" named "name".
		 *  The position of the next sibling (and all successive
		 *  siblings) is decremented. */
		bool	deleteFirstChild(const char *ns, const char *name);

		/** Deletes the first child named "name" (ignoring case).
		 *  The position of the next sibling (and all successive
		 *  siblings) is decremented. */
		bool	deleteFirstChildIgnoringCase(const char *name);

		/** Deletes the first child in namespace "ns" named "name"
		 *  (both ignoring case).  The position of the next sibling
		 *  (and all successive siblings) is decremented. */
		bool	deleteFirstChildIgnoringCase(const char *ns,
							const char *name);

		/** Deletes the first child node named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteFirstChild(const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes the first child node in namespace "ns"
		 *  named "name" with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes the first child node named "name" 
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		bool	deleteFirstChildIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes the first child node in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		bool	deleteFirstChildIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all child nodes. */
		bool	deleteChildren();

		/** Deletes all child nodes named "name". */
		bool	deleteChildren(const char *name);

		/** Deletes all child nodes in namespace "ns"
		 *  named "name". */
		bool	deleteChildren(const char *ns, const char *name);

		/** Deletes all child nodes named "name" (ignoring case). */
		bool	deleteChildrenIgnoringCase(const char *name);

		/** Deletes all child nodes in namespace "ns"
		 *  named "name" (both ignoring case). */
		bool	deleteChildrenIgnoringCase(const char *ns,
							const char *name);

		/** Deletes all child nodes named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteChildren(const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all child nodes in namespace "ns"
		 *  named "name" with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the child node is not
		 *  checked, and the first child node with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteChildren(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all child nodes named "name" 
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the child node
		 *  is not checked, and the first child node with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		bool	deleteChildrenIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all child nodes in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute name "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.  If
		 *  "name" is null, then the name of the child node is not
		 *  checked, and the first child node with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		bool	deleteChildrenIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all descendents named "name". */
		bool	deleteDescendents(const char *name);

		/** Deletes all descendents in namespace "ns"
		 *  named "name". */
		bool	deleteDescendents(const char *ns,
						const char *name);

		/** Deletes all descendents named "name" (ignoring case). */
		bool	deleteDescendentsIgnoringCase(const char *name);

		/** Deletes all descendents in namespace "ns" named "name"
		 *  (both ignoring case). */
		bool	deleteDescendentsIgnoringCase(const char *ns,
							const char *name);

		/** Deletes all descendents named "name"
		 *  with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the descendent is not
		 *  checked, and the first descendent with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteDescendents(const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all descendents in namespace "ns"
		 *  named "name" with an attribute named "attributename" with
		 *  value "attributevalue" whose type is
		 *  TAG_XMLDOMNODE.  If "name" is null,
		 *  then the name of the descendent is not
		 *  checked, and the first descendent with any
		 *  name (with matching attribute name/value)
		 *  will be returned.  If no match is found,
		 *  nullnode is returned. */
		bool	deleteDescendents(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all descendents named "name" 
		 *  (ignoring case) with an attribute named
		 *  "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.
		 *  If "name" is null, then the name of the descendent
		 *  is not checked, and the first descendent with any
		 *  name (with matching attribute name/value) will be
		 *  returned.  If no match is found, nullnode is returned. */
		bool	deleteDescendentsIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue);

		/** Deletes all descendents in namespace "ns"
		 *  (ignoring case) named "name" (ignoring case) with an
		 *  attribute named "attributename" with value "attributevalue"
		 *  (both ignoring case) whose type is TAG_XMLDOMNODE.  If
		 *  "name" is null, then the name of the descendent is not
		 *  checked, and the first descendent with any name (with
		 *  matching attribute name/value) will be returned.  If no
		 *  match is found, nullnode is returned. */
		bool	deleteDescendentsIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue);


		/** Finds the first child node named "oldname" and
		 *  renames it "newname". */
		bool	renameFirstChild(const char *oldname,
						const char *newname);


		/** Finds the first child node named "oldname" in namespace
		 *  "oldns" and renames it "newname" with namespace "newns". */
		bool	renameFirstChild(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname);

		/** Finds the first child node named "oldname" (ignoring case)
		 *  and renames it "newname". */
		bool	renameFirstChildIgnoringCase(const char *oldname,
							const char *newname);

		/** Finds the first child node named "oldname" in namespace
		 *  "oldns" (both ignoring case) and renames it "newname"
		 *  with namespace "newns". */
		bool	renameFirstChildIgnoringCase(const char *oldns,
							const char *oldname,
							const char *newns,
							const char *newname);

		/** Finds all child nodes named "oldname" and renames
		 *  them "newname". */
		bool	renameChildren(const char *oldname,
						const char *newname);

		/** Finds all child nodes named "oldname" in namespace "oldns"
		 *  and renames them "newname" with namespace "newns". */
		bool	renameChildren(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname);

		/** Finds all child nodes named "oldname" (ignoring
		 *  case) and renames them "newname". */
		bool	renameChildrenIgnoringCase(const char *oldname,
							const char *newname);

		/** Finds all child nodes named "oldname" in namespace "oldns"
		 *  (both ignoring case) and renames them "newname" with
		 *  namespace "newns". */
		bool	renameChildrenIgnoringCase(const char *oldns,
							const char *oldname,
							const char *newns,
							const char *newname);

		/** Finds all descendents named "oldname" and renames
		 *  them "newname". */
		bool	renameDescendents(const char *oldname,
						const char *newname);

		/** Finds all descendents named "oldname" in namespace "oldns"
 		 *  and renames them "newname" with namespace "newns". */
		bool	renameDescendents(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname);

		/** Finds all descendents named "oldname" (ignoring
 		 *  case) and renames them "newname". */
		bool	renameDescendentsIgnoringCase(
						const char *oldname,
						const char *newname);

		/** Finds all descendents named "oldname" in namespace "oldns"
		 *  (both ignoring case) and renames them "newname" with
		 *  namespace "newns". */
		bool	renameDescendentsIgnoringCase(
						const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname);


		/** Inserts a child node of type TEXT_XMLDOMNODE with
		 *  value "value" into the list of child nodes at
		 *  "position".  The position of the next sibling
		 *  (and all successive siblings) is incremented. */
		bool	insertText(const char *value, uint64_t position);

		/** Appends a child node of type TEXT_XMLDOMNODE with
		 *  value "value" to the list of child nodes. */
		bool	appendText(const char *value);

		/** Inserts "attribute" into the list of attributes at
		 *  "position".  The position of the next attribute
		 *  (and all successive attributes) is incremented. */
		bool	insertAttribute(xmldomnode *attribute,
							uint64_t position);

		/** Appends "attribute" to the list of attributes. */
		bool	appendAttribute(xmldomnode *attribute);

		/** Creates an attribute node with "name" and "value"
		 *  and inserts it into the list of attributes at
		 *  "position".  The position of the next attribute
		 *  (and all successive attributes) is incremented. */
		bool	insertAttribute(const char *name, const char *value,
							uint64_t position);

		/** Creates an attribute node with "name" and "value"
		 *  and appends it to the list of attributes. */
		bool	appendAttribute(const char *name, const char *value);

		/** Deletes the attribute at "position".  The position
		 *  of the next attribute (and all successive attributes)
		 *  is decremented. */
		bool	deleteAttribute(uint64_t position);

		/** Searches the list of attribute nodes for an attribute
		 *  named "name" and deletes it.  The position of the
		 *  next attribute (and all successive attributes) is
		 *  decremented. */
		bool	deleteAttribute(const char *name);

		/** Searches the list of attribute nodes for "attribute"
		 *  and deletes it.  The position of the next attribute
		 *  (and all successive attributes) is decremented. */
		bool	deleteAttribute(xmldomnode *attribute);


		/** Moves all children of "child" out of "child" and then
		 *  deletes "child".
		 *
		 *  The behavior is undefined if "child" is not actually a
		 *  child of this node. */
		bool	unwrapChild(xmldomnode *child);

		/** Finds the first tag child named "name", moves all
		 *  children out of the tag, then deletes the tag. */
		bool	unwrapFirstChild(const char *name);

		/** Finds the first tag child in namespace "ns" named "name",
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapFirstChild(const char *ns, const char *name);

		/** Finds the first tag child named "name" (ignoring case),
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapFirstChildIgnoringCase(const char *name);

		/** Finds the first tag child in namespace "ns" named "name"
		 *  (both ignoring case), moves all children out of the tag,
		 *  then deletes the tag. */
		bool	unwrapFirstChildIgnoringCase(const char *ns,
							const char *name);

		/** For each tag child named "name", moves all children
		 *  out of the tag, then deletes the tag. */
		bool	unwrapChildren(const char *name);

		/** For each tag child in namespace "ns" named "name",
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapChildren(const char *ns, const char *name);

		/** For each tag child named "name" (ignoring case),
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapChildrenIgnoringCase(const char *name);

		/** For each tag child in namespace "ns" named "name"
		 *  (both ignoring case), moves all children out of the tag,
		 *  then deletes the tag. */
		bool	unwrapChildrenIgnoringCase(const char *ns,
							const char *name);

		/** For each tag descendent named "name", moves all children
		 *  out of the tag, then deletes the tag. */
		bool	unwrapDescendents(const char *name);

		/** For each tag descendent in namespace "ns" named "name",
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapDescendents(const char *ns, const char *name);

		/** For each tag descendent named "name" (ignoring case),
		 *  moves all children out of the tag, then deletes the tag. */
		bool	unwrapDescendentsIgnoringCase(const char *name);

		/** For each tag descendent in namespace "ns" named "name"
		 *  (both ignoring case), moves all children out of the tag,
		 *  then deletes the tag. */
		bool	unwrapDescendentsIgnoringCase(const char *ns,
							const char *name);


		/** Wraps the specified child with a tag named "name".
		 *  Returns the newly new created tag node.
		 * 
		 *  The behavior is undefined if "child" is not actually a
		 *  child of this node. */
		xmldomnode	*wrapChild(xmldomnode *child,
						const char *name);

		/** Wraps the specified child with a tag named "name"
		 *  with namespace "ns".
		 *  Returns the newly new created tag node.
		 * 
		 *  The behavior is undefined if "child" is not actually a
		 *  child of this node. */
		xmldomnode	*wrapChild(xmldomnode *child,
						const char *ns,
						const char *name);

		/** Wraps the specified children with a tag named "name".
		 *  Returns the newly new created tag node.
		 *
		 *  "startchild" and "endchild" should be children of the
		 *  current node, and siblings, and not the null node.  The
		 *  position of "endchild" should be >= the position of
		 *  "startchild".  The behavior is undefined if any of
		 *  these conditions are not met. */
		xmldomnode	*wrapChildren(xmldomnode *startchild,
						xmldomnode *endchild,
						const char *name);

		/** Wraps the specified children with a tag named "name"
 		 *  with namespace "ns".
		 *  Returns the newly new created tag node.
		 *
		 *  "startchild" and "endchild" should be children of the
		 *  current node, and siblings, and not the null node.  The
		 *  position of "endchild" should be >= the position of
		 *  "startchild".  The behavior is undefined if any of
		 *  these conditions are not met. */
		xmldomnode	*wrapChildren(xmldomnode *startchild,
						xmldomnode *endchild,
						const char *ns,
						const char *name);

		/** Wraps all children of the current node with a tag
		 *  named "name".  Returns the newly new created tag node. */
		xmldomnode	*wrapChildren(const char *name);

		/** Wraps all children of the current node with a tag
		 *  named "name" with namespace "ns".
		 *  Returns the newly new created tag node. */
		xmldomnode	*wrapChildren(const char *ns, const char *name);


		/** Clones this node and all of its children and returns a
		 *  pointer to the clone. */
		xmldomnode	*clone();

		/** Clones this node and all of its children in tree "dom"
		 *  and returns a pointer to the clone. */
		xmldomnode	*clone(xmldom *dom);


		/** Allocates a stringbuffer, writes a text
		 *  representation of the tree starting at this
		 *  node to it and returns the stringbuffer;
		 *  The calling program must deallocate the
		 *  stringbuffer. */
		stringbuffer	*xml() const;

		/** Prints a text representation of the tree
		 *  starting at this node into "strb". */
		void	print(stringbuffer *strb) const;

		/** Prints a text representation of the tree
		 *  starting at this node to filedescriptor "fd". */
		void	print(filedescriptor *fd) const;

		/** If the xmldomnode is an element, returns the
		 *  "path" of the xmldomnode.  The path will have
		 *  the following form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  The return value is allocated inside the
		 *  method and must be deallocated by the calling
		 *  program. */
		stringbuffer	*getPath() const;

		/** Returns the child element with "path" of the form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  Returns the null node if the specified
		 *  element was not found. */
		xmldomnode	*getChildByPath(const char *path) const;

		/** Returns the attribute node at index
		 *  "position" of the child element with "path"
		 *  of the form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  Returns the null node if the specified
		 *  element was not found. */
		xmldomnode	*getAttributeByPath(const char *path,
						uint64_t position) const;

		/** Returns the attribute node named "name"
		 *  of the child element with "path" of the form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  Returns the null node if the specified
		 *  element was not found. */
		xmldomnode	*getAttributeByPath(const char *path,
							const char *name) const;

		/** Returns the value of the attribute at index
		 *  "position" of the child element with "path"
		 *  of the form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  Returns the null node if the specified
		 *  element was not found. */
		const char	*getAttributeValueByPath(const char *path,
						uint64_t position) const;

		/** Returns the value of the attribute named
		 *  "name" of the child element with "path" of
		 *  the form:
		 * 
		 *  /element[index]/element[index]/...
		 * 
		 *  Returns the null node if the specified
		 *  element was not found. */
		const char	*getAttributeValueByPath(const char *path,
							const char *name) const;

		/** Allows an app to associate arbitrary data with the node. */
		void	setData(void *data);

		/** Allows an app to get the arbitrary data associated with
		 *  the node using setData(). */
		void	*getData();

	#include <rudiments/private/xmldomnode.h>
};

#endif
