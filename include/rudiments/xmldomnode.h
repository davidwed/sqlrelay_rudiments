// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOMNODE_H
#define RUDIMENTS_XMLDOMNODE_H

#include <rudiments/private/xmldomnodeincludes.h>

// The xmldomnode class provides a generic container for DOM tree elements.
// One can navigate the nodes of the tree, modify the tree and read or modify
// the data that the nodes contain by calling methods in this class.
//
// A DOM tree node can be one of the following:
// 	the document root
// 	a tag 
//	a tag attribute
//	a segment of text
//	a comment
//	a segment of CDATA
//
// Each node may contain the following data, though for some node types, the
// data container is unused:
// 	type
// 	name
// 	value
// 	parent node
// 	next sibling
// 	previous sibling
// 	a list of attribute nodes
// 	a list of child nodes
// 	
// Here is a breakdown by node type:
//
// 	For the document root:
// 		type - ROOT_XMLDOMNODETYPE
// 		name - "document"
// 		value - unused
// 		parent node - unused
// 		next sibling - unused
// 		previous sibling - unused
// 		a list of attribute nodes - unused
// 		a list of child nodes -	the xml version tag, the doctype tag
// 					and the top-level enclosing tag
//
// 	For a tag:
// 		type - TAG_XMLDOMNODETYPE
// 		name - the tag name
// 		value - unused
// 		parent node - the parent tag or document root
// 		next sibling - can be another tag, a segment of text,
// 					 a comment or a segment of cdata
// 		previous sibling - can be another tag, a segment of text,
// 					 a comment or a segment of cdata
// 		a list of attribute nodes - a list of attribute nodes
// 		a list of child nodes - a list of tags, text segments, comments
// 					and/or cdata segments
//
// 	For a tag attribute:
// 		type - ATTRIBUTE_XMLDOMNODETYPE
// 		name - the attribute name
// 		value - the attribute value
// 		    (note that for tags with standalone
// 		    attributes, the name and value are the same)
// 		parent node - the tag containing the attributes
// 		next sibling - the next attribute
// 		previous sibling - the previous attribute
// 		a list of attribute nodes - unused
// 		a list of child nodes - unused
//
// 	For a segment of text:
// 		type - TEXT_XMLDOMNODETYPE
// 		name - "text"
// 		value - the text itself
// 		parent node - the tag containing the text
// 		next sibling - can be a tag, a comment or a segment of cdata
// 		previous sibling - can be a tag, a comment or a segment of cdata
// 		a list of attribute nodes - unused
// 		a list of child nodes - unused
//
// 	For a comment:
// 		type - COMMENT_XMLDOMNODETYPE
// 		name - "comment"
// 		value - the comment itself
// 		parent node - the tag containing the comment
// 		next sibling - can be a tag, a segment of text, another
// 				comment or a segment of cdata
// 		previous sibling - can be a tag, a segment of text, another
// 				comment or a segment of cdata
// 		a list of attribute nodes - unused
// 		a list of child nodes - unused
//
// 	For a segment of cdata:
// 		type - CDATA_XMLDOMNODETYPE
// 		name - "cdata"
// 		value - the cdata itself
// 		parent node - the tag containing the cdata
// 		next sibling - can be a tag, a segment of text, a comment
// 				or another segment of cdata
// 		previous sibling - can be a tag, a segment of text, a comment
// 				or another segment of cdata
// 		a list of attribute nodes - unused
// 		a list of child nodes - unused



// node types
enum xmldomnodetype {
	ROOT_XMLDOMNODETYPE=1,
	TAG_XMLDOMNODETYPE,
	ATTRIBUTE_XMLDOMNODETYPE,
	TEXT_XMLDOMNODETYPE,
	COMMENT_XMLDOMNODETYPE,
	CDATA_XMLDOMNODETYPE
};

class xmldomnode {
	public:
			xmldomnode(xmldomnode *nullnode);
				// Creates a new node and intializes its
				// member variables to NULL.
				//
				// Your application should pass in a special
				// "nullnode" which may be created by the
				// static method createNullNode() below.
				//
				// This will keep command chaining like this:
				//
				// mynode->getChild("node1")->
				// 	getChild("node2")->getName("node3");
				//
				// from causing the program to crash trying to
				// dereference a NULL pointer if, for example,
				// "node2" doesn't exist.
			xmldomnode(xmldomnode *nullnode,
					xmldomnodetype type,
					const char *name, const char *value);
				// Creates a new node (as above) and intializes
				// its member variables to the values passed in.
			~xmldomnode();
				// Deletes the node, all attribute nodes and
				// optionally all child nodes, recursively.



		static	xmldomnode	*createNullNode();
					// Creates a special "null node" whose
					// parent, next sibling and previous
					// siblings point back to itself.
					//
					// This special node should be passed
					// in when creating new xmldomnodes.
					//
					// This method allocates xmldomnode
					// internally and passes a pointer
					// back.  The calling program must
					// ultimately deallocate the node.


		// These methods control the behavior when the node is deleted.
		void		cascadeOnDelete();
				// Instructs the destructor to recursively
				// delete all child nodes. (the default)
		void		dontCascadeOnDelete();
				// Instructs the destructor not to recursively
				// delete all child nodes.


		// These methods provide read-access to the data
		// contained in the node.
		xmldomnodetype	getType() const;
				// Returns the node type.
		char		*getName() const;
				// Returns the node name.
		char		*getValue() const;
				// Returns the node value.


		xmldomnode	*getParent() const;
				// Returns a pointer to the parent node or the
				// nullnode if none exists.
		xmldomnode	*getPreviousSibling() const;
				// Returns a pointer to the previous sibling
				// node or the nullnode if none exists.
		xmldomnode	*getPreviousTagSibling() const;
				// Returns a pointer to the previous sibling
				// node whose type is TAG_XMLDOMNODE.  If no
				// match is found, nullnode is returned.
		xmldomnode	*getPreviousTagSibling(const char *name) const;
				// Returns the previous sibling node named
				// "name" whose type is TAG_XMLDOMNODE or the
				// nullnode if not found.
		xmldomnode	*getPreviousTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const;
				// Returns the previous sibling node named
				// "name" with an attribute named
				// "attributename" with value "attributevalue"
				// whose type is TAG_XMLDOMNODE.  If "name" is
				// null, then the name of the child node is not
				// checked, and the first child node with any
				// name (with matching attribute name/value)
				// will be returned.  If no match is found,
				// nullnode is returned.
		xmldomnode	*getNextSibling() const;
				// Returns a pointer to the next sibling node
				// or the nullnode if none exists.
		xmldomnode	*getNextTagSibling() const;
				// Returns a pointer to the next sibling node
				// whose type is TAG_XMLDOMNODE.  If no match
				// is found, nullnode is returned.
		xmldomnode	*getNextTagSibling(const char *name) const;
				// Returns the next sibling node named "name"
				// whose type is TAG_XMLDOMNODE or the nullnode
				// if not found.
		xmldomnode	*getNextTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const;
				// Returns the next sibling node named "name"
				// with an attribute named "attributename" with
				// value "attributevalue" whose type is
				// TAG_XMLDOMNODE.  If "name" is null,
				// then the name of the child node is not
				// checked, and the first child node with any
				// name (with matching attribute name/value)
				// will be returned.  If no match is found,
				// nullnode is returned.


		int		getChildCount() const;
				// Returns the number of immediate child nodes.
		xmldomnode	*getChild(const char *name) const;
				// Returns the child node named "name"
				// or the nullnode if not found.
		xmldomnode	*getChild(int position) const;
				// Returns the child node at index "position"
				// or the nullnode if not found.
		xmldomnode	*getChild(const char *name,
						const char *attributename,
						const char *attributevalue)
						const;
				// Returns the first child node named "name"
				// with an attribute named "attributename" with
				// value "attributevalue".  If "name" is null,
				// then the name of the child node is not
				// checked, and the first child node with any
				// name (with matching attribute name/value)
				// will be returned.  If no match is found,
				// nullnode is returned.
		xmldomnode	*getFirstTagChild() const;
				// Returns the first child node whose type is
				// TAG_XMLDOMNODE.  If no match is found,
				// nullnode is returned.


		int		getAttributeCount() const;
				// Returns the number of attributes.
		xmldomnode	*getAttribute(const char *name) const;
				// Returns the attribute named "name"
				// or the nullnode if not found.
		xmldomnode	*getAttribute(int position) const;
				// Returns the attribute node at index
				// "position" or the nullnode if not found.
		char		*getAttributeValue(const char *name) const;
				// Returns the value of the attribute named
				// "name" or the nullnode if not found.
		char		*getAttributeValue(int position) const;
				// Returns the value of the attribute node at
				// index "position" or the nullnode if not
				// found.
		namevaluepairs	*getAttributes() const;
				// Returns the attribute names and values in
				// a namevaluepairs dictionary.  The instance
				// of namevaluepairs is allocated internally
				// and must be deleted by the calling program.					// Returns NULL if the node is a nullNode and
				// an empty dictionary if the node has no
				// attributes.

		xmldomnode	*getNullNode() const;
				// Returns the nullnode used by this node.
		bool		isNullNode() const;
				// Returns true if this node is the special
				// nullnode and false otherwise.


		// These methods provide write-access to the data contained in
		// the node.  These methods can also be used to move nodes
		// around in the tree, and insert or delete them.
		void	setType(xmldomnodetype type);
			// Sets the node type to "type".
		void	setName(const char *name);
			// Sets the node name to "name".
		void	setValue(const char *value);
			// Sets the node value to "value".

		void	setParent(xmldomnode *parent);
			// Sets the parent of the node to "parent".
		void	setPreviousSibling(xmldomnode *previous);
			// Sets the previous sibling of the node to "previous".
		void	setNextSibling(xmldomnode *next);
			// Sets the next sibling of the node to "next".

		bool	insertChild(xmldomnode *child, int position);
			// Inserts "child" into the list of child nodes at
			// "position".  The position of the next sibling
			// (and all successive siblings) is incremented.
		bool	appendChild(xmldomnode *child);
			// Appends "child" to the list of child nodes.
		bool	deleteChild(int position);
			// Deletes the child node at "position".  The position
			// of the next sibling (and all successive siblings)
			// is decremented.
		bool	deleteChild(xmldomnode *child);
			// Searches the list of child nodes for "child" and
			// deletes it.  The position of the next sibling (and
			// all successive siblings) is decremented.

		bool	insertText(const char *value, int position);
			// Inserts a child node of type TEXT_XMLDOMNODE with
			// value "value" into the list of child nodes at
			// "position".  The position of the next sibling
			// (and all successive siblings) is incremented.
		bool	appendText(const char *value);
			// Appends a child node of type TEXT_XMLDOMNODE with
			// value "value" to the list of child nodes.

		bool	insertAttribute(xmldomnode *attribute, int position);
			// Inserts "attribute" into the list of attributes at
			// "position".  The position of the next attribute
			// (and all successive attributes) is incremented.
		bool	appendAttribute(xmldomnode *attribute);
			// Appends "attribute" to the list of attributes.
		bool	insertAttribute(const char *name, const char *value,
					int position);
			// Creates an attribute node with "name" and "value"
			// and inserts it into the list of attributes at
			// "position".  The position of the next attribute
			// (and all successive attributes) is incremented.
		bool	appendAttribute(const char *name, const char *value);
			// Creates an attribute node with "name" and "value"
			// and appends it to the list of attributes.
		bool	deleteAttribute(int position);
			// Deletes the attribute at "position".  The position
			// of the next attribute (and all successive attributes)
			// is decremented.
		bool	deleteAttribute(const char *name);
			// Searches the list of attribute nodes for an attribute
			// named "name" and deletes it.  The position of the
			// next attribute (and all successive attributes) is
			// decremented.
		bool	deleteAttribute(xmldomnode *attribute);
			// Searches the list of attribute nodes for "attribute"
			// and deletes it.  The position of the next attribute
			// (and all successive attributes) is decremented.


		stringbuffer	*xml() const;
				// Allocates a stringbuffer, writes a textual
				// representation of the tree starting at this
				// node to it and returns the stringbuffer;
				// The calling program must deallocate the
				// stringbuffer.

		stringbuffer	*getPath() const;
				// If the xmldomnode is an element, returns the
				// "path" of the xmldomnode.  The path will have
				// the following form:
				//
				// /element[index]/element[index]/...
				//
				// The return value is allocated inside the
				// method and must be deallocated by the calling
				// program.
		xmldomnode	*getChildByPath(const char *path) const;
				// Returns the child element with "path" of the
				// form:
				//
				// /element[index]/element[index]/...
				//
				// Returns the null node if the specified
				// element was not found.

	#include <rudiments/private/xmldomnode.h>
};

#endif
