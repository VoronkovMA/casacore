//# ExprNode.h: Handle class for a table column expression tree
//# Copyright (C) 1994,1995,1996,1997,1998,2000,2001,2003
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA

#ifndef TABLES_EXPRNODE_H
#define TABLES_EXPRNODE_H

//# Includes
#include <casacore/casa/aips.h>
#include <casacore/tables/TaQL/ExprNodeRep.h>
#include <casacore/tables/TaQL/ExprRange.h>
#include <casacore/tables/TaQL/ExprFuncNode.h>
#include <casacore/tables/TaQL/TaQLStyle.h>
#include <casacore/tables/TaQL/MArray.h>
#include <casacore/tables/Tables/RowNumbers.h>
#include <casacore/casa/Arrays/ArrayFwd.h>
#include <casacore/casa/Utilities/DataType.h>
#include <casacore/casa/BasicSL/Complex.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

//# Forward Declarations
class Table;
class String;
class Regex;
class StringDistance;
class Unit;
class Record;
class TableRecord;
class TableExprNodeBinary;
class TableExprNodeSet;
template<class T> class Block;
template<class T> class MArray;


// <summary>
// Handle class for a table column expression tree
// </summary>

// <use visibility=export>

// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="">
// </reviewed>

// <prerequisite>
//# Classes you should understand before using this one.
//   <li> <linkto class=Table>Table</linkto>
//   <li> Note 199 describing
//        <a href="../notes/199.html">
//        TaQL</a>
// </prerequisite>

// <etymology>
// TableExprNode represents a node in the tree reflecting a
// table select expression.
// </etymology>

// <synopsis>
// TableExprNode is the class to store a table select expression,
// making it possible to select rows from the table. The selected
// rows form a table which is a view of the original table.
// <p>
// TableExprNode is a handle class for the counted referenced class
// TableExprNodeRep.
// Classes (like TableExprNodePlusXX) derived from TableExprNodeRep
// hold the individual
// nodes in the expression, i.e. the operators and operands. The nodes
// form a binary tree reflecting the expression.
// E.g. the expression 2*COLUMN results in the node TableExprNodeTimes
// with its children TableExprNodeConst and TableExprNodeColumn.
// Constant subexpressions (like 2*3) are evaluated immediately and
// only the result is stored as a node.
// <p>
// There are a few TableExprNode constructors taking a constant scalar or array.
// In this way constant value are automatically converted to the
// appropriate TableExprNodeConst object.
// <p>
// The derived classes also reflect the data type of the node.
// Data types Bool, Int64, Double, DComplex and String are used.
// Char, uChar, Short, uShort, Int and uInt are converted to Int64,
// float to Double, and Complex to DComplex.
// Binary operators +, -, *, /, %, &, }, ^, ==, >=, >, <, <= and != are
// recognized. Also &&, ||, parentheses and unary +, -, ~ and ! are recognized.
// For strings the binary operator + can also be used.
// The operators have the normal C++ precedence.
// Furthermore functions (such as sin, max, ceil) can be used in an expression.
// <br>Operator() can be used to take a slice from an array.
// <p>
// The Table function col has to be used to create a TableExprNode
// object for a column in the table. The Table
// <linkto file="Table.h#keycol">operator()</linkto> can be used
// the do the actual selection from the top TableExprNode object.
// </synopsis>

// <example>
// <srcblock>
//   // Select from table X all rows where column RA<5 and where column
//   // SWITCH is true.
//   Table table("X");
//   Table subtable = table(table.col("RA") < 5 && table.col("SWITCH"));
//
//   // Select from that result all rows where the concatenation of
//   // the strings in columns STR1 and STR2 is equal to the string
//   // in keyword STRKEY.
//   Table subsub = subtable(subtable.col("STR1") + subtable.col("STR2")
//                           == subtable.key("STRKEY"));
// </srcblock>
// </example>

// <motivation>
// Having TableExprNode as a handle class makes it possible to
// handle temporary objects created by the compiler in a smooth way.
// TableExprNode and its derivations allow to store an expression
// before actually evaluating it. This also allows the classes to
// be used by the table expression parser defined in TableParse and
// TableGram.
//
// For each operator a special derived class is implemented.
// Another approach could have been to store the operator as
// a flag and switch on that. However, that causes extra overhead
// and the C++ virtual function mechanism is the designed for
// these purposes.
// </motivation>

// <todo asof="$DATE:$">
//# A List of bugs, limitations, extensions or planned refinements.
//   <li> add operations on arrays
//   <li> add selection by comparing with a set of values
// </todo>


class TableExprNode
{
public:
    TableExprNode ();

    // Unary operators on numeric TableExprNode's.
    // <group>
    TableExprNode operator+ () const;
    TableExprNode operator- () const;
    // </group>
    // Unary NOT-operator on boolean TableExprNode's.
    TableExprNode operator! () const;
    // Unary bitwise negate-operator on integer TableExprNode's.
    TableExprNode operator~ () const;

    // Slicing in a node containing an array. It is possible to
    // address a single pixel or an n-dimensional subarray.
    // In case of a single pixel the result is a scalar node.
    // Otherwise the result is an array node with the same dimensionality
    // as the source.
    // <br>Note that there exist TableExprNodeSet constructors to
    // convert an <src>IPosition</src> or <src>Slicer</src> object
    // automatically to a <src>TableExprNodeSet</src>.
    // An <src>IPosition</src> addresses a single element and results in
    // a scalar node, while a <src>Slicer</src> can address multiple
    // elements and always results in an array node.
    TableExprNode operator() (const TableExprNodeSet& indices);

    // The IN operator to test if a value is contained in an array or set.
    // The array can also be a scalar.
    // <group>
    TableExprNode in (const TableExprNode& array,
                      const TaQLStyle& = TaQLStyle(0)) const;
    TableExprNode in (const TableExprNodeSet& set,
                      const TaQLStyle& = TaQLStyle(0)) const;
    // </group>

    // Use a unit for the given TableExprNode.
    // Note that if a column has a unit, it is automatically set. In that case
    // this can be used to convert units.
    TableExprNode useUnit (const Unit& unit) const;

    // Constructors to convert a constant value to a TableExprNode.
    // The constructor for char* is also supported to convert a
    // character-array to a string, since a two step conversion
    // is not done automatically.
    // <group>
    TableExprNode (const Bool& value);
    TableExprNode (const Int& value);
    TableExprNode (const uInt& value);
    TableExprNode (const Int64& value);
    TableExprNode (const uInt64& value);
    TableExprNode (const Float& value);
    TableExprNode (const Double& value);
    TableExprNode (const Complex& value);
    TableExprNode (const DComplex& value);
    TableExprNode (const String& value);
    TableExprNode (const std::string& value);
    TableExprNode (const char*);
    TableExprNode (const Regex& value);
    TableExprNode (const StringDistance& value);
    TableExprNode (const TaqlRegex& value);
    TableExprNode (const MVTime& value);
    TableExprNode (const Array<Bool>& value);
    TableExprNode (const Array<uChar>& value);
    TableExprNode (const Array<Short>& value);
    TableExprNode (const Array<uShort>& value);
    TableExprNode (const Array<Int>& value);
    TableExprNode (const Array<uInt>& value);
    TableExprNode (const Array<Int64>& value);
    //# The following constructor has to be explicit, othwerwise
    //#    Table(Vector<rownr_t>)
    //# gives an ambiguous error as the preferred class RowNumbers
    //# has a similar constructor.
    explicit TableExprNode (const Array<uInt64>& value);
    TableExprNode (const Array<Float>& value);
    TableExprNode (const Array<Double>& value);
    TableExprNode (const Array<Complex>& value);
    TableExprNode (const Array<DComplex>& value);
    TableExprNode (const Array<String>& value);
    TableExprNode (const Array<MVTime>& value);
    TableExprNode (const MArray<Bool>& value);
    TableExprNode (const MArray<uChar>& value);
    TableExprNode (const MArray<Short>& value);
    TableExprNode (const MArray<uShort>& value);
    TableExprNode (const MArray<Int>& value);
    TableExprNode (const MArray<uInt>& value);
    TableExprNode (const MArray<Int64>& value);
    TableExprNode (const MArray<uInt64>& value);
    TableExprNode (const MArray<Float>& value);
    TableExprNode (const MArray<Double>& value);
    TableExprNode (const MArray<Complex>& value);
    TableExprNode (const MArray<DComplex>& value);
    TableExprNode (const MArray<String>& value);
    TableExprNode (const MArray<MVTime>& value);
    // </group>

    // Construct a node from a node representation shared pointer
    // which increments the reference count.
    TableExprNode (const TENShPtr&);

    // Construct from a node representation. It takes over the pointer, so the
    // object gets deleted automatically.
    TableExprNode (TableExprNodeRep* rep)
      : node_p(TENShPtr(rep)) {}

    // copy constructor (reference semantics).
    TableExprNode (const TableExprNode&);

    // Assignment (reference semantics).
    TableExprNode& operator= (const TableExprNode&);

    // The destructor deletes all the underlying TableExprNode objects,
    ~TableExprNode ();

    // Does the node contain no actual node?
    Bool isNull() const
      { return !node_p; }

    // Do not apply the selection.
    void disableApplySelection()
      { node_p->disableApplySelection(); }

    // Re-create the column object for a selection of rows.
    // Nothing is done if the node does not represent a column object.
    void applySelection (const Vector<rownr_t>& rownrs)
      { node_p->applySelection (rownrs); }

    // Get the table info of the expression node.
    TableExprInfo getTableInfo() const
      { return node_p->getTableInfo(); }

    // Get the table to which the expression node belongs.
    //# [[deprecated ("Use getTableInfo().table() instead")]]
    Table table() const
      __attribute__ ((deprecated ("Use getTableInfo().table() instead")))
      { return getTableInfo().table(); }

    // Get the unit of the expression.
    const Unit& unit() const
      { return node_p->unit(); }

    // Get the attributes of the expression.
    const Record& attributes() const
      { return node_p->attributes(); }

    // Get the data type of the expression.
    // Currently the only possible values are TpBool, TpInt, TpDouble,
    // TpDComplex, TpString, and TpOther.
    // The latter is returned for a date or regex.
    DataType dataType() const;

    // Is the expression a scalar?
    Bool isScalar() const
      { return (node_p->valueType() == TableExprNodeRep::VTScalar); }

    // Get the number of rows in the table associated with this expression.
    // One is returned if the expression is a constant or if no table is
    // associated with it.
    rownr_t nrow() const
      { return node_p->nrow(); }

    // Get a value for this node in the given row.
    // These functions are implemented in the derived classes and
    // will usually invoke the get in their children and apply the
    // operator on the resulting values.
    // <group>
    void get (const TableExprId& id, Bool& value) const;
    void get (const TableExprId& id, Int64& value) const;
    void get (const TableExprId& id, Double& value) const;
    void get (const TableExprId& id, DComplex& value) const;
    void get (const TableExprId& id, String& value) const;
    void get (const TableExprId& id, TaqlRegex& value) const;
    void get (const TableExprId& id, MVTime& value) const;
    void get (const TableExprId& id, MArray<Bool>& value) const;
    void get (const TableExprId& id, MArray<Int64>& value) const;
    void get (const TableExprId& id, MArray<Double>& value) const;
    void get (const TableExprId& id, MArray<DComplex>& value) const;
    void get (const TableExprId& id, MArray<String>& value) const;
    void get (const TableExprId& id, MArray<MVTime>& value) const;
    void get (const TableExprId& id, Array<Bool>& value) const;
    void get (const TableExprId& id, Array<Int64>& value) const;
    void get (const TableExprId& id, Array<Double>& value) const;
    void get (const TableExprId& id, Array<DComplex>& value) const;
    void get (const TableExprId& id, Array<String>& value) const;
    void get (const TableExprId& id, Array<MVTime>& value) const;
    Bool     getBool     (const TableExprId& id) const;
    Int64    getInt      (const TableExprId& id) const;
    Double   getDouble   (const TableExprId& id) const;
    DComplex getDComplex (const TableExprId& id) const;
    MVTime   getDate     (const TableExprId& id) const;
    String   getString   (const TableExprId& id) const;
    Array<Bool>     getArrayBool     (const TableExprId& id) const;
    Array<Int64>    getArrayInt      (const TableExprId& id) const;
    Array<Double>   getArrayDouble   (const TableExprId& id) const;
    Array<DComplex> getArrayDComplex (const TableExprId& id) const;
    Array<String>   getArrayString   (const TableExprId& id) const;
    Array<MVTime>   getArrayDate     (const TableExprId& id) const;
    // Get a value as an array, even it it is a scalar.
    // This is useful in case one can give an argument as scalar or array.
    // <group>
    MArray<Bool>     getBoolAS     (const TableExprId& id) const;
    MArray<Int64>    getIntAS      (const TableExprId& id) const;
    MArray<Double>   getDoubleAS   (const TableExprId& id) const;
    MArray<DComplex> getDComplexAS (const TableExprId& id) const;
    MArray<String>   getStringAS   (const TableExprId& id) const;
    MArray<MVTime>   getDateAS     (const TableExprId& id) const;
    // </group>

    // </group>

    // Get the data type for doing a getColumn on the expression.
    // This is the data type of the column if the expression
    // consists of a single column only.
    // Otherwise it is the expression data type as returned by
    // function <src>dataType</src>.
    DataType getColumnDataType() const;

    // Get the value of the expression evaluated for the entire column.
    // The data of function called should match the data type as
    // returned by function <src>getColumnDataType</src>.
    // <group>
    Array<Bool>     getColumnBool (const RowNumbers& rownrs) const;
    Array<uChar>    getColumnuChar (const RowNumbers& rownrs) const;
    Array<Short>    getColumnShort (const RowNumbers& rownrs) const;
    Array<uShort>   getColumnuShort (const RowNumbers& rownrs) const;
    Array<Int>      getColumnInt (const RowNumbers& rownrs) const;
    Array<uInt>     getColumnuInt (const RowNumbers& rownrs) const;
    Array<Int64>    getColumnInt64 (const RowNumbers& rownrs) const;
    Array<Float>    getColumnFloat (const RowNumbers& rownrs) const;
    Array<Double>   getColumnDouble (const RowNumbers& rownrs) const;
    Array<Complex>  getColumnComplex (const RowNumbers& rownrs) const;
    Array<DComplex> getColumnDComplex (const RowNumbers& rownrs) const;
    Array<String>   getColumnString (const RowNumbers& rownrs) const;
    // </group>

    // The same functions as above for the old Vector<uInt>.
    // They are primarily meant for CASA's SplatalogueTable class.
    // Normally they should not be used, hence declared private
    // unless told otherwise.
#ifndef IMPLICIT_CTDS_32BIT
private:
#endif
    // <group>
    Array<Bool>     getColumnBool (const Vector<uInt>& rownrs) const
      { return getColumnBool (RowNumbers(rownrs)); }
    Array<uChar>    getColumnuChar (const Vector<uInt>& rownrs) const
      { return getColumnuChar (RowNumbers(rownrs)); }
    Array<Short>    getColumnShort (const Vector<uInt>& rownrs) const
      { return getColumnShort (RowNumbers(rownrs)); }
    Array<uShort>   getColumnuShort (const Vector<uInt>& rownrs) const
      { return getColumnuShort (RowNumbers(rownrs)); }
    Array<Int>      getColumnInt (const Vector<uInt>& rownrs) const
      { return getColumnInt (RowNumbers(rownrs)); }
    Array<uInt>     getColumnuInt (const Vector<uInt>& rownrs) const
      { return getColumnuInt (RowNumbers(rownrs)); }
    Array<Int64>    getColumnInt64 (const Vector<uInt>& rownrs) const
      { return getColumnInt64 (RowNumbers(rownrs)); }
    Array<Float>    getColumnFloat (const Vector<uInt>& rownrs) const
      { return getColumnFloat (RowNumbers(rownrs)); }
    Array<Double>   getColumnDouble (const Vector<uInt>& rownrs) const
      { return getColumnDouble (RowNumbers(rownrs)); }
    Array<Complex>  getColumnComplex (const Vector<uInt>& rownrs) const
      { return getColumnComplex (RowNumbers(rownrs)); }
    Array<DComplex> getColumnDComplex (const Vector<uInt>& rownrs) const
      { return getColumnDComplex (RowNumbers(rownrs)); }
    Array<String>   getColumnString (const Vector<uInt>& rownrs) const
      { return getColumnString (RowNumbers(rownrs)); }
    // </group>
public:

    // Show the tree.
    void show (ostream&) const;

    // Convert the tree to a number of range vectors which at least
    // select the same things.
    // This function is very useful to convert the expression to
    // some intervals covering the select expression. This can
    // be used to do a rough fast selection via an index and do the
    // the slower final selection on that much smaller subset.
    // The function can only convert direct comparisons of columns
    // with constants (via ==, !=, >, >=, < or <=) and their combinations
    // using && or ||.
    void ranges (Block<TableExprRange>&);

    // Check if tables used in expression have the same number of
    // rows as the given table.
    Bool checkTableSize (const Table& table, Bool canBeConst) const;

    // Create a column node or constant keyword node.
    static TableExprNode keyCol (const TableExprInfo& tabInfo,
                                 const String& name,
                                 const Vector<String>& fieldNames);

    // Create a column node on behalf of the Table class.
    // <src>fieldNames</src> indicate a possible field in a column of Records.
    // For builtin data types another type of node is created than
    // for other data types.
    static TableExprNode newColumnNode (const TableExprInfo&,
                                        const String& colName,
                                        const Vector<String>& fieldNames);

    // Create a TableExprNodeConst for a table keyword
    // (which is handled as a constant).
    // <src>fieldNames</src> tells the name of the keyword and possible subrecords.
    static TableExprNode newKeyConst (const TableRecord&,
                                      const Vector<String>& fieldNames);

    // Handle all field names except the last one. ALl of them must
    // be records. The last record is returned.
    // fullName is filled with the full keyword name separated by dots.
    static TableRecord* findLastKeyRec (const TableRecord& keyset,
                                        const Vector<String>& fieldNames,
                                        String& fullName);

    // Throw invalid data type exception.
    static void throwInvDT (const String& message);

    // Create function node of the given type with the given arguments.
    // <group>
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNodeSet& set,
                                          const TableExprInfo& tabInfo,
                                          const TaQLStyle& = TaQLStyle(0));
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNode& node);
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNode& node1,
                                          const TableExprNode& node2);
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNode& node1,
                                          const TableExprNode& node2,
                                          const TableExprNode& node3);
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNode& array,
                                          const TableExprNodeSet& axes);
    static TableExprNode newFunctionNode (TableExprFuncNode::FunctionType,
                                          const TableExprNode& array,
                                          const TableExprNode& node,
                                          const TableExprNodeSet& axes);
    // </group>

    // Create a user defined function node.
    static TableExprNode newUDFNode (const String& name,
                                     const TableExprNodeSet& set,
                                     const TableExprInfo& tableInfo,
                                     const TaQLStyle& = TaQLStyle(0));

    // Create cone function node of the given type with the given arguments.
    // <group>
    static TableExprNode newConeNode (TableExprFuncNode::FunctionType,
                                      const TableExprNodeSet& set,
                                      uInt origin = 0);
    static TableExprNode newConeNode (TableExprFuncNode::FunctionType,
                                      const TableExprNode& node1,
                                      const TableExprNode& node2);
    static TableExprNode newConeNode (TableExprFuncNode::FunctionType,
                                      const TableExprNode& node1,
                                      const TableExprNode& node2,
                                      const TableExprNode& node3);
    // </group>

    // Create rownumber() function node.
    // Origin indicates whether the first row should be zero (for C++ binding)
    // or an other value (one for TaQL binding).
    static TableExprNode newRownrNode (const TableExprInfo&, uInt origin);

    // Create rowid() function node.
    // Origin is always 0.
    static TableExprNode newRowidNode (const TableExprInfo&);

    // Create rand() function node.
    static TableExprNode newRandomNode (const TableExprInfo&);

    // Create ArrayElement node for the given array with the given index.
    // The origin is 0 for C++ and 1 for TaQL.
    static TableExprNode newArrayPartNode (const TableExprNode& arrayNode,
                                           const TableExprNodeSet& indices,
                                           const TaQLStyle& = TaQLStyle(0));

    // returns const pointer to the underlying TableExprNodeRep object.
    const TENShPtr& getRep() const;
    const TableExprNodeRep* getNodeRep() const;

    // Adapt the unit of the expression to the given unit (if not empty).
    void adaptUnit (const Unit&);

    // Construct a new node for the given operation.
    // <group>
    TENShPtr newPlus   (const TENShPtr& right) const;
    TENShPtr newMinus  (const TENShPtr& right) const;
    TENShPtr newTimes  (const TENShPtr& right) const;
    TENShPtr newDivide (const TENShPtr& right) const;
    TENShPtr newModulo (const TENShPtr& right) const;
    TENShPtr newBitAnd (const TENShPtr& right) const;
    TENShPtr newBitOr  (const TENShPtr& right) const;
    TENShPtr newBitXor (const TENShPtr& right) const;
    TENShPtr newEQ     (const TENShPtr& right) const;
    TENShPtr newNE     (const TENShPtr& right) const;
    TENShPtr newGE     (const TENShPtr& right) const;
    TENShPtr newGT     (const TENShPtr& right) const;
    TENShPtr newIN     (const TENShPtr& right, const TaQLStyle&) const;
    TENShPtr newOR     (const TENShPtr& right) const;
    TENShPtr newAND    (const TENShPtr& right) const;
    // </group>

private:
    // Put the new binary node object in a shared pointer.
    // Set the node's info and adapt the children if needed.
    // If the node is constant, it is evaluated and returned as result.
    TENShPtr setBinaryNodeInfo (TableExprNodeBinary* tsnptr,
                                const TENShPtr& right=TENShPtr()) const;

    // convert Block of TableExprNode to vector of TENShPtr.
    static std::vector<TENShPtr> convertBlockTEN (Block<TableExprNode>& nodes);

    // The actual (counted referenced) representation of a node.
    TENShPtr node_p;
};



inline void TableExprNode::ranges (Block<TableExprRange>& blrange)
    { node_p->ranges (blrange); }

//# Get the value of an expression.
inline void TableExprNode::get (const TableExprId& id, Bool& value) const
    { value = node_p->getBool (id); }
inline void TableExprNode::get (const TableExprId& id, Int64& value) const
    { value = node_p->getInt (id); }
inline void TableExprNode::get (const TableExprId& id, Double& value) const
    { value = node_p->getDouble (id); }
inline void TableExprNode::get (const TableExprId& id, DComplex& value) const
    { value = node_p->getDComplex (id); }
inline void TableExprNode::get (const TableExprId& id, String& value) const
    { value = node_p->getString (id); }
inline void TableExprNode::get (const TableExprId& id, TaqlRegex& value) const
    { value = node_p->getRegex (id); }
inline void TableExprNode::get (const TableExprId& id, MVTime& value) const
    { value = node_p->getDate (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<Bool>& value) const
    { value = node_p->getArrayBool (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<Int64>& value) const
    { value = node_p->getArrayInt (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<Double>& value) const
    { value = node_p->getArrayDouble (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<DComplex>& value) const
    { value = node_p->getArrayDComplex (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<String>& value) const
    { value = node_p->getArrayString (id); }
inline void TableExprNode::get (const TableExprId& id,
                                MArray<MVTime>& value) const
    { value = node_p->getArrayDate (id); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<Bool>& value) const
    { value = node_p->getArrayBool (id).array(); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<Int64>& value) const
    { value = node_p->getArrayInt (id).array(); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<Double>& value) const
    { value = node_p->getArrayDouble (id).array(); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<DComplex>& value) const
    { value = node_p->getArrayDComplex (id).array(); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<String>& value) const
    { value = node_p->getArrayString (id).array(); }
inline void TableExprNode::get (const TableExprId& id,
                                Array<MVTime>& value) const
    { value = node_p->getArrayDate (id).array(); }
inline Bool TableExprNode::getBool (const TableExprId& id) const
    { return node_p->getBool (id); }
inline Int64 TableExprNode::getInt (const TableExprId& id) const
    { return node_p->getInt (id); }
inline Double TableExprNode::getDouble (const TableExprId& id) const
    { return node_p->getDouble (id); }
inline DComplex TableExprNode::getDComplex (const TableExprId& id) const
    { return node_p->getDComplex (id); }
inline MVTime TableExprNode::getDate (const TableExprId& id) const
    { return node_p->getDate (id); }
inline String TableExprNode::getString (const TableExprId& id) const
    { return node_p->getString (id); }
inline Array<Bool> TableExprNode::getArrayBool (const TableExprId& id) const
    { return node_p->getArrayBool (id).array(); }
inline Array<Int64> TableExprNode::getArrayInt (const TableExprId& id) const
    { return node_p->getArrayInt (id).array(); }
inline Array<Double> TableExprNode::getArrayDouble (const TableExprId& id) const
    { return node_p->getArrayDouble (id).array(); }
inline Array<DComplex> TableExprNode::getArrayDComplex (const TableExprId& id) const
    { return node_p->getArrayDComplex (id).array(); }
inline Array<String> TableExprNode::getArrayString (const TableExprId& id) const
    { return node_p->getArrayString (id).array(); }
inline Array<MVTime> TableExprNode::getArrayDate (const TableExprId& id) const
    { return node_p->getArrayDate (id).array(); }
inline MArray<Bool> TableExprNode::getBoolAS (const TableExprId& id) const
    { return node_p->getBoolAS (id); }
inline MArray<Int64> TableExprNode::getIntAS (const TableExprId& id) const
    { return node_p->getIntAS (id); }
inline MArray<Double> TableExprNode::getDoubleAS (const TableExprId& id) const
    { return node_p->getDoubleAS (id); }
inline MArray<DComplex> TableExprNode::getDComplexAS (const TableExprId& id) const
    { return node_p->getDComplexAS (id); }
inline MArray<String> TableExprNode::getStringAS (const TableExprId& id) const
    { return node_p->getStringAS (id); }
inline MArray<MVTime> TableExprNode::getDateAS (const TableExprId& id) const
    { return node_p->getDateAS (id); }

inline Array<Bool>      TableExprNode::getColumnBool (const RowNumbers& rownrs) const
    { return node_p->getColumnBool (rownrs); }
inline Array<uChar>     TableExprNode::getColumnuChar (const RowNumbers& rownrs) const
    { return node_p->getColumnuChar (rownrs); }
inline Array<Short>     TableExprNode::getColumnShort (const RowNumbers& rownrs) const
    { return node_p->getColumnShort (rownrs); }
inline Array<uShort>    TableExprNode::getColumnuShort (const RowNumbers& rownrs) const
    { return node_p->getColumnuShort (rownrs); }
inline Array<Int>       TableExprNode::getColumnInt (const RowNumbers& rownrs) const
    { return node_p->getColumnInt (rownrs); }
inline Array<uInt>      TableExprNode::getColumnuInt (const RowNumbers& rownrs) const
    { return node_p->getColumnuInt (rownrs); }
inline Array<Int64>     TableExprNode::getColumnInt64 (const RowNumbers& rownrs) const
    { return node_p->getColumnInt64 (rownrs); }
inline Array<Float>     TableExprNode::getColumnFloat (const RowNumbers& rownrs) const
    { return node_p->getColumnFloat (rownrs); }
inline Array<Double>    TableExprNode::getColumnDouble (const RowNumbers& rownrs) const
    { return node_p->getColumnDouble (rownrs); }
inline Array<Complex>   TableExprNode::getColumnComplex (const RowNumbers& rownrs) const
    { return node_p->getColumnComplex (rownrs); }
inline Array<DComplex>  TableExprNode::getColumnDComplex (const RowNumbers& rownrs) const
    { return node_p->getColumnDComplex (rownrs); }
inline Array<String>    TableExprNode::getColumnString (const RowNumbers& rownrs) const
    { return node_p->getColumnString (rownrs); }

inline void TableExprNode::show (ostream& os) const
    { node_p->show (os, 0); }
inline const TENShPtr& TableExprNode::getRep() const
    { return node_p; }
inline const TableExprNodeRep* TableExprNode::getNodeRep() const
    { return node_p.get(); }



// Define all global functions operating on a TableExprNode.
// <group name=GlobalTableExprNode>

  //# Define the operations we allow.
  //# Note that the arguments are defined as const. This is necessary
  //# because the compiler generates temporaries when converting a constant
  //# to a TableExprNode using the constructors. Temporaries has to be const.
  //# However, we have to delete created nodes, so lnode_p and rnode_p
  //# cannot be const. The const arguments are casted to a non-const in
  //# the function fill which calls the non-const function simplify.

  // Arithmetic operators for numeric TableExprNode's.
  // <group>
    // + is also defined for strings (means concatenation).
    TableExprNode operator+ (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator- (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator* (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator/ (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator% (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator& (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator| (const TableExprNode& left,
                             const TableExprNode& right);
    TableExprNode operator^ (const TableExprNode& left,
                             const TableExprNode& right);
  // </group>

  // Comparison operators.
  // <group>
    TableExprNode operator== (const TableExprNode& left,
                              const TableExprNode& right);
    TableExprNode operator!= (const TableExprNode& left,
                              const TableExprNode& right);
    // Not defined for Bool.
    // <group>
    TableExprNode operator>= (const TableExprNode& left,
                              const TableExprNode& right);
    TableExprNode operator>  (const TableExprNode& left,
                              const TableExprNode& right);
    TableExprNode operator<= (const TableExprNode& left,
                              const TableExprNode& right);
    TableExprNode operator<  (const TableExprNode& left,
                              const TableExprNode& right);
    // </group>
  // </group>

  // Logical operators to combine boolean TableExprNode's.
  // A null TableExprNode object is ignored, so it is possible to
  // build up a full expression gradually.
  // <group>
    TableExprNode operator&& (const TableExprNode& left,
                              const TableExprNode& right);
    TableExprNode operator|| (const TableExprNode& left,
                              const TableExprNode& right);
  // </group>

  // Functions to return whether a value is "relatively" near another.
  // Returns <src> tol > abs(val2 - val1)/max(abs(val1),(val2))</src>.
  // If tol <= 0, returns val1 == val2. If either val is 0.0, takes
  // care of area around the minimum number that can be represented.
  // <br>The nearAbs functions return whether a value is "absolutely" near
  // another. Returns <src> tol > abs(val2 - val1)</src>.
  // Default tolerance is 1.0e-13.
  // They operate on scalars and arrays.
  // <group>
    TableExprNode near    (const TableExprNode& left,
                           const TableExprNode& right);
    TableExprNode near    (const TableExprNode& left,
                           const TableExprNode& right,
                           const TableExprNode& tolerance);
    TableExprNode nearAbs (const TableExprNode& left,
                           const TableExprNode& right);
    TableExprNode nearAbs (const TableExprNode& left,
                           const TableExprNode& right,
                           const TableExprNode& tolerance);
  // </group>

  // Angular distance between positions.
  // Both arguments have to be arrays. If both arrays contain 2 values
  // (ra and dec), the result is a scalar.
  // Otherwise the arrays have to contain a multiple of 2 values and the
  // result is a 2-dim array giving the distance of each position in the
  // first array to each position in the second array.
    TableExprNode angdist (const TableExprNode& pos1,
                           const TableExprNode& pos2);

  // Angular distance as above, but only pair-wise enties are used if
  // both arguments are arrays.
    TableExprNode angdistx (const TableExprNode& pos1,
                            const TableExprNode& pos2);

  // Cone search; test if the position of a source is inside a cone.
  // <br>Argument <src>sourcePos</src> must be a double array
  // containing two values (ra and dec of source) in radians.
  // <br>Argument <src>cones</src> must be a double array
  // specifying the position of the cone centers and radii in radians.
  // So the array must contain three values (ra,dec,radius)
  // or a multiple of it.
  // <group>
    // The result is a bool array telling for each cone if it contains the
    // source. If there is only one cone, the result is a scalar.
    TableExprNode cones (const TableExprNode& sourcePos,
                         const TableExprNode& cones);
    // The result is always a Bool scalar telling if any cone contains
    // the source.
    TableExprNode anyCone (const TableExprNode& sourcePos,
                           const TableExprNode& cones);
    // The sourcePos can contain multiple sources.
    // The result is a double array giving the index of the first
    // cone containing the corresponding source.
    // If there is one source, the result is a double scalar.
    TableExprNode findCone (const TableExprNode& sourcePos,
                            const TableExprNode& cones);
  // </group>

  // Cone search as above.
  // However, the cone positions and radii are specified separately
  // and (virtually) a larger array containing every combination of
  // position/radius is formed.
  // <group>
    TableExprNode cones (const TableExprNode& sourcePos,
                         const TableExprNode& conePos,
                         const TableExprNode& radii);
    TableExprNode anyCone (const TableExprNode& sourcePos,
                           const TableExprNode& conePos,
                           const TableExprNode& radii);
    TableExprNode findCone (const TableExprNode& sourcePos,
                            const TableExprNode& conePos,
                            const TableExprNode& radii);
  // </group>

  // Transcendental functions that can be applied to essentially all numeric
  // nodes containing scalars or arrays.
  // <group>
    TableExprNode sin    (const TableExprNode& node);
    TableExprNode sinh   (const TableExprNode& node);
    TableExprNode cos    (const TableExprNode& node);
    TableExprNode cosh   (const TableExprNode& node);
    TableExprNode exp    (const TableExprNode& node);
    TableExprNode log    (const TableExprNode& node);
    TableExprNode log10  (const TableExprNode& node);
    TableExprNode pow    (const TableExprNode& x, const TableExprNode& exp);
    TableExprNode square (const TableExprNode& node);
    TableExprNode cube   (const TableExprNode& node);
    TableExprNode sqrt   (const TableExprNode& node);
    TableExprNode norm   (const TableExprNode& node);
  // </group>

  // Transcendental functions applied to to nodes containing scalars or
  // arrays with double values.
  // They are invalid for Complex nodes.
  // <group>
    TableExprNode asin  (const TableExprNode& node);
    TableExprNode acos  (const TableExprNode& node);
    TableExprNode atan  (const TableExprNode& node);
    TableExprNode atan2 (const TableExprNode& y,
                         const TableExprNode& x);
    TableExprNode tan   (const TableExprNode& node);
    TableExprNode tanh  (const TableExprNode& node);
    TableExprNode sign  (const TableExprNode& node);
    TableExprNode round (const TableExprNode& node);
    TableExprNode ceil  (const TableExprNode& node);
    TableExprNode abs   (const TableExprNode& node);
    TableExprNode floor (const TableExprNode& node);
    TableExprNode fmod  (const TableExprNode& x,
                         const TableExprNode& y);
  // </group>

  // String functions on scalars or arrays.
  // <group>
    TableExprNode strlength (const TableExprNode& node);
    TableExprNode upcase    (const TableExprNode& node);
    TableExprNode downcase  (const TableExprNode& node);
    TableExprNode capitalize(const TableExprNode& node);
    TableExprNode trim      (const TableExprNode& node);
    TableExprNode ltrim     (const TableExprNode& node);
    TableExprNode rtrim     (const TableExprNode& node);
    TableExprNode substr    (const TableExprNode& str,
                             const TableExprNode& pos);
    TableExprNode substr    (const TableExprNode& str,
                             const TableExprNode& pos,
                             const TableExprNode& npos);
    TableExprNode replace   (const TableExprNode& str,
                             const TableExprNode& patt);
    TableExprNode replace   (const TableExprNode& str,
                             const TableExprNode& patt,
                             const TableExprNode& repl);
  // </group>

  // Functions for regular expression matching and
  // pattern matching. Defined for scalars and arrays.
  // <br><src>pattern</src> is for a file name like pattern.
  // <br><src>sqlpattern</src> is for an SQL like pattern.
  // <group>
    TableExprNode regex      (const TableExprNode& node);
    TableExprNode pattern    (const TableExprNode& node);
    TableExprNode sqlpattern (const TableExprNode& node);
  // </group>

  // Functions for date-values. Defined for scalars and arrays.
  //# Note, ctod is called ctodt, because Mac OS-X defines a macro
  //# ctod in param.h
  // <group>
    TableExprNode datetime  (const TableExprNode& node);
    TableExprNode mjdtodate (const TableExprNode& node);
    TableExprNode mjd       (const TableExprNode& node);
    TableExprNode date      (const TableExprNode& node);
    TableExprNode year      (const TableExprNode& node);
    TableExprNode month     (const TableExprNode& node);
    TableExprNode day       (const TableExprNode& node);
    TableExprNode cmonth    (const TableExprNode& node);
    TableExprNode weekday   (const TableExprNode& node);
    TableExprNode cdow      (const TableExprNode& node);
    TableExprNode ctodt     (const TableExprNode& node);
    TableExprNode cdate     (const TableExprNode& node);
    TableExprNode ctime     (const TableExprNode& node);
    TableExprNode week            (const TableExprNode& node);
    TableExprNode time      (const TableExprNode& node);
  // </group>

  // Functions for angle-values. Defined for scalars and arrays.
  // dhms converts pairs of values to hms and dms and only works for arrays.
  // <group>
    TableExprNode hms  (const TableExprNode& node);
    TableExprNode dms  (const TableExprNode& node);
    TableExprNode hdms (const TableExprNode& node);
  // </group>

  // Function to convert any value to a string.
  // See TaQL note 199 for possible format values.
  // <group>
    TableExprNode toString (const TableExprNode& node);
    TableExprNode toString (const TableExprNode& node,
                            const TableExprNode& format);
  // </group>

  // Function to test if a scalar or array is NaN (not-a-number).
  // It results in a Bool scalar or array.
    TableExprNode isNaN (const TableExprNode& node);

  // Function to test if a scalar or array is finite.
  // It results in a Bool scalar or array.
    TableExprNode isFinite (const TableExprNode& node);

  // Minimum or maximum of 2 nodes.
  // Makes sense for numeric and String values. For Complex values
  // the norm is compared.
  // One or both arguments can be scalar or array.
  // <group>
    TableExprNode min (const TableExprNode& a, const TableExprNode& b);
    TableExprNode max (const TableExprNode& a, const TableExprNode& b);
  // </group>

  // The complex conjugate of a complex node.
  // Defined for scalars and arrays.
    TableExprNode conj (const TableExprNode& node);

  // The real part of a complex node.
  // Defined for scalars and arrays.
    TableExprNode real (const TableExprNode& node);

  // The imaginary part of a complex node.
  // Defined for scalars and arrays.
    TableExprNode imag (const TableExprNode& node);

  // Convert double, bool, or string to int (using floor).
    TableExprNode integer (const TableExprNode& node);

  // Convert numeric or string value to bool (0, no, false, - means false)
    TableExprNode boolean (const TableExprNode& node);

  // The amplitude (i.e. sqrt(re*re + im*im)) of a complex node.
  // This is a synonym for function abs.
  // Defined for scalars and arrays.
    TableExprNode amplitude (const TableExprNode& node);

  // The phase (i.e. atan2(im, re)) of a complex node.
  // This is a synonym for function arg.
  // Defined for scalars and arrays.
    TableExprNode phase (const TableExprNode& node);

  // The arg (i.e. atan2(im, re)) of a complex node.
  // Defined for scalars and arrays.
    TableExprNode arg (const TableExprNode& node);

  // Form a complex number from two Doubles.
  // One or both arguments can be scalar or array.
    TableExprNode formComplex (const TableExprNode& real,
                               const TableExprNode& imag);
  // Form a complex number from a string.
  // Defined for scalars and arrays.
    TableExprNode formComplex (const TableExprNode& node);

  // Functions operating on a Double or Complex scalar or array resulting in
  // a scalar with the same data type.
  // <group>
    TableExprNode sum (const TableExprNode& array);
    TableExprNode product (const TableExprNode& array);
    TableExprNode sumSquare (const TableExprNode& array);
  // </group>

  // Functions operating on a Double scalar or array resulting in
  // a Double scalar.
  // <group>
    TableExprNode min (const TableExprNode& array);
    TableExprNode max (const TableExprNode& array);
    TableExprNode mean (const TableExprNode& array);
    TableExprNode variance (const TableExprNode& array);
    TableExprNode stddev (const TableExprNode& array);
    TableExprNode avdev (const TableExprNode& array);
    TableExprNode rms (const TableExprNode& array);
    TableExprNode median (const TableExprNode& array);
    TableExprNode fractile (const TableExprNode& array,
                            const TableExprNode& fraction);
  // </group>

  // <group>
    TableExprNode any (const TableExprNode& array);
    TableExprNode all (const TableExprNode& array);
    TableExprNode ntrue (const TableExprNode& array);
    TableExprNode nfalse (const TableExprNode& array);
  // </group>

  // The partial version of the functions above.
  // They are applied to the array subsets defined by the axes in the set
  // using the partialXXX functions in ArrayMath.
  // The axes must be 0-relative.
  // <group>
    TableExprNode sums (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode products (const TableExprNode& array,
                            const TableExprNodeSet& collapseAxes);
    TableExprNode sumSquares (const TableExprNode& array,
                              const TableExprNodeSet& collapseAxes);
    TableExprNode mins (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode maxs (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode means (const TableExprNode& array,
                         const TableExprNodeSet& collapseAxes);
    TableExprNode variances (const TableExprNode& array,
                             const TableExprNodeSet& collapseAxes);
    TableExprNode stddevs (const TableExprNode& array,
                           const TableExprNodeSet& collapseAxes);
    TableExprNode avdevs (const TableExprNode& array,
                          const TableExprNodeSet& collapseAxes);
    TableExprNode rmss (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode medians (const TableExprNode& array,
                           const TableExprNodeSet& collapseAxes);
    TableExprNode fractiles (const TableExprNode& array,
                             const TableExprNode& fraction,
                             const TableExprNodeSet& collapseAxes);
    TableExprNode anys (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode alls (const TableExprNode& array,
                        const TableExprNodeSet& collapseAxes);
    TableExprNode ntrues (const TableExprNode& array,
                          const TableExprNodeSet& collapseAxes);
    TableExprNode nfalses (const TableExprNode& array,
                           const TableExprNodeSet& collapseAxes);
  // </group>

  // Functions operating for each element on a box around that element.
  // The elements at the edges (where no full box can be made) are set to 0.
  // <group>
    TableExprNode runningMin (const TableExprNode& array,
                              const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningMax (const TableExprNode& array,
                              const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningMean (const TableExprNode& array,
                               const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningVariance (const TableExprNode& array,
                                   const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningStddev (const TableExprNode& array,
                                 const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningAvdev (const TableExprNode& array,
                                const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningRms (const TableExprNode& array,
                              const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningMedian (const TableExprNode& array,
                                 const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningAny (const TableExprNode& array,
                              const TableExprNodeSet& halfBoxWidth);
    TableExprNode runningAll (const TableExprNode& array,
                              const TableExprNodeSet& halfBoxWidth);
  // </group>

  // Create an array of the given shape and fill it with the values.
  // The <src>values</src> array is rewound as needed.
    TableExprNode array (const TableExprNode& values,
                         const TableExprNodeSet& shape);

  // Form a masked array.
    TableExprNode marray (const TableExprNode& array,
                          const TableExprNode& mask);

  // Get the data array of a masked array.
    TableExprNode arrayData (const TableExprNode& array);

  // Flatten a masked array (get unmasked elements).
    TableExprNode arrayFlatten (const TableExprNode& array);

  // Get the mask of a masked array.
  // If the array has no mask, it return an array with all False values.
    TableExprNode arrayMask (const TableExprNode& array);

  // Get the diagonal of a (masked) array;
  // If the array is not a Matrix, it will take the diagonals of the
  // subarrays given by the two axes in the axes argument. Those
  // axes have to have the same length (thus each subarray is a Matrix).
  // If no axes are given, they default to the first two axes.
  // <br>The <src>diag</src> argument tells which diagonal to take.
  // 0 is the main diagonal, >0 is above main diagonal, <0 is below.
    TableExprNode diagonal (const TableExprNode& array);
    TableExprNode diagonal (const TableExprNode& array,
                            const TableExprNode& firstAxis);
    TableExprNode diagonal (const TableExprNode& array,
                            const TableExprNode& firstAxis,
                            const TableExprNode& diag);

  // Transpose all axes of a (masked) array.
    TableExprNode transpose (const TableExprNode& array);
  // Transpose a (masked) array by making the given axes the first axes.
    TableExprNode transpose (const TableExprNode& array,
                             const TableExprNode& axes);

  // Function operating on a field resulting in a bool scalar.
  // It can be used to test if a column has an array in the current row.
  // It can also be used to test if a record contains a field.
    TableExprNode isdefined (const TableExprNode& array);

  // Functions operating on any scalar or array resulting in a Double scalar.
  // A scalar has 1 element and dimensionality 0.
  // <group>
    TableExprNode nelements (const TableExprNode& array);
    TableExprNode ndim (const TableExprNode& array);
  // </group>

  // Function operating on any scalar or array resulting in a Double array
  // containing the shape. A scalar has shape [1].
    TableExprNode shape (const TableExprNode& array);

  // Function resembling the ternary <src>?:</src> construct in C++.
  // The argument "condition" has to be a Bool value.
  // If an element in "condition" is True, the corresponding element from
  // "arg1" is taken, otherwise it is taken from "arg2".
  // The arguments can be scalars or array or any combination.
    TableExprNode iif (const TableExprNode& condition,
                       const TableExprNode& arg1,
                       const TableExprNode& arg2);
// </group>



inline TableExprNode operator+ (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newPlus (right.getRep());
}
inline TableExprNode operator- (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newMinus (right.getRep());
}
inline TableExprNode operator* (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newTimes (right.getRep());
}
inline TableExprNode operator/ (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newDivide (right.getRep());
}
inline TableExprNode operator% (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newModulo (right.getRep());
}
inline TableExprNode operator& (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newBitAnd (right.getRep());
}
inline TableExprNode operator| (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newBitOr (right.getRep());
}
inline TableExprNode operator^ (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newBitXor (right.getRep());
}
inline TableExprNode operator== (const TableExprNode& left,
                                 const TableExprNode& right)
{
    return left.newEQ (right.getRep());
}
inline TableExprNode operator!= (const TableExprNode& left,
                                 const TableExprNode& right)
{
    return left.newNE (right.getRep());
}
inline TableExprNode operator> (const TableExprNode& left,
                                const TableExprNode& right)
{
    return left.newGT (right.getRep());
}
inline TableExprNode operator>= (const TableExprNode& left,
                                 const TableExprNode& right)
{
    return left.newGE (right.getRep());
}
inline TableExprNode operator<= (const TableExprNode& left,
                                 const TableExprNode& right)
{
    return right.newGE (left.getRep());
}
inline TableExprNode operator< (const TableExprNode& left,
                                const TableExprNode& right)
{
    return right.newGT (left.getRep());
}
inline TableExprNode TableExprNode::in (const TableExprNode& right,
                                        const TaQLStyle& style) const
{
    return newIN (right.getRep(), style);
}
inline TableExprNode TableExprNode::operator() (const TableExprNodeSet& indices)
{
    // C++ indexing is 0-based.
    return newArrayPartNode (*this, indices, TaQLStyle(0));
}

inline TableExprNode near (const TableExprNode& left,
                           const TableExprNode& right)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::near2FUNC,
                                           left, right);
}
inline TableExprNode near (const TableExprNode& left,
                           const TableExprNode& right,
                           const TableExprNode& tolerance)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::near3FUNC,
                                           left, right, tolerance);
}
inline TableExprNode nearAbs (const TableExprNode& left,
                              const TableExprNode& right)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::nearabs2FUNC,
                                           left, right);
}
inline TableExprNode nearAbs (const TableExprNode& left,
                              const TableExprNode& right,
                              const TableExprNode& tolerance)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::nearabs3FUNC,
                                           left, right, tolerance);
}
inline TableExprNode angdist (const TableExprNode& pos1,
                              const TableExprNode& pos2)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::angdistFUNC,
                                           pos1, pos2);
}
inline TableExprNode angdistx (const TableExprNode& pos1,
                               const TableExprNode& pos2)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::angdistxFUNC,
                                           pos1, pos2);
}
inline TableExprNode cones (const TableExprNode& sourcePos,
                            const TableExprNode& cones)
{
    return TableExprNode::newConeNode (TableExprFuncNode::conesFUNC,
                                       sourcePos, cones);
}
inline TableExprNode anyCone (const TableExprNode& sourcePos,
                              const TableExprNode& cones)
{
    return TableExprNode::newConeNode (TableExprFuncNode::anyconeFUNC,
                                       sourcePos, cones);
}
inline TableExprNode findCone (const TableExprNode& sourcePos,
                               const TableExprNode& cones)
{
    return TableExprNode::newConeNode (TableExprFuncNode::findconeFUNC,
                                       sourcePos, cones);
}
inline TableExprNode cones (const TableExprNode& sourcePos,
                            const TableExprNode& conePos,
                            const TableExprNode& radii)
{
    return TableExprNode::newConeNode (TableExprFuncNode::cones3FUNC,
                                       sourcePos, conePos, radii);
}
inline TableExprNode anyCone (const TableExprNode& sourcePos,
                              const TableExprNode& conePos,
                              const TableExprNode& radii)
{
    return TableExprNode::newConeNode (TableExprFuncNode::anycone3FUNC,
                                       sourcePos, conePos, radii);
}
inline TableExprNode findCone (const TableExprNode& sourcePos,
                               const TableExprNode& conePos,
                               const TableExprNode& radii)
{
    return TableExprNode::newConeNode (TableExprFuncNode::findcone3FUNC,
                                       sourcePos, conePos, radii);
}
inline TableExprNode cos (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::cosFUNC, node);
}
inline TableExprNode cosh (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::coshFUNC, node);
}
inline TableExprNode exp (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::expFUNC, node);
}
inline TableExprNode log (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::logFUNC, node);
}
inline TableExprNode log10 (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::log10FUNC, node);
}
inline TableExprNode pow (const TableExprNode& x, const TableExprNode& y)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::powFUNC, x, y);
}
inline TableExprNode sin (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::sinFUNC, node);
}
inline TableExprNode sinh (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::sinhFUNC, node);
}
inline TableExprNode square (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::squareFUNC,
                                           node);
}
inline TableExprNode cube (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::cubeFUNC,
                                           node);
}
inline TableExprNode sqrt (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::sqrtFUNC, node);
}
inline TableExprNode norm (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::normFUNC, node);
}
inline TableExprNode acos (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::acosFUNC, node);
}
inline TableExprNode asin (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::asinFUNC, node);
}
inline TableExprNode atan (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::atanFUNC, node);
}
inline TableExprNode atan2 (const TableExprNode& y, const TableExprNode& x)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::atan2FUNC, y, x);
}
inline TableExprNode sign (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::signFUNC, node);
}
inline TableExprNode round (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::roundFUNC, node);
}
inline TableExprNode ceil (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::ceilFUNC, node);
}
inline TableExprNode abs (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::absFUNC, node);
}
inline TableExprNode floor (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::floorFUNC, node);
}
inline TableExprNode fmod (const TableExprNode& x, const TableExprNode& y)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::fmodFUNC, x, y);
}
inline TableExprNode tan (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::tanFUNC, node);
}
inline TableExprNode tanh (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::tanhFUNC, node);
}
inline TableExprNode min (const TableExprNode& a, const TableExprNode& b)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::minFUNC, a, b);
}
inline TableExprNode max (const TableExprNode& a, const TableExprNode& b)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::maxFUNC, a, b);
}
inline TableExprNode real (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::realFUNC, node);
}
inline TableExprNode imag (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::imagFUNC, node);
}
inline TableExprNode integer (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::intFUNC, node);
}
inline TableExprNode boolean (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boolFUNC, node);
}
inline TableExprNode conj (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::conjFUNC, node);
}
inline TableExprNode amplitude (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::absFUNC, node);
}
inline TableExprNode arg (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::argFUNC, node);
}
inline TableExprNode phase (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::argFUNC, node);
}
inline TableExprNode formComplex (const TableExprNode& real,
                                  const TableExprNode& imag)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::complexFUNC,
                                           real, imag);
}
inline TableExprNode formComplex (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::complexFUNC,
                                           node);
}
inline TableExprNode strlength (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::strlengthFUNC,
                                           node);
}
inline TableExprNode upcase (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::upcaseFUNC,
                                           node);
}
inline TableExprNode downcase (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::downcaseFUNC,
                                           node);
}
inline TableExprNode capitalize (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::capitalizeFUNC,
                                           node);
}
inline TableExprNode regex (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::regexFUNC, node);
}
inline TableExprNode pattern (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::patternFUNC,
                                           node);
}
inline TableExprNode sqlpattern (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::sqlpatternFUNC,
                                           node);
}
inline TableExprNode datetime (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::datetimeFUNC,
                                           node);
}
inline TableExprNode mjdtodate (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::mjdtodateFUNC,
                                           node);
}
inline TableExprNode mjd (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::mjdFUNC, node);
}
inline TableExprNode date (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::dateFUNC, node);
}
inline TableExprNode year (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::yearFUNC, node);
}
inline TableExprNode month (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::monthFUNC, node);
}
inline TableExprNode day (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::dayFUNC, node);
}
inline TableExprNode cmonth (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::cmonthFUNC,
                                           node);
}
inline TableExprNode weekday (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::weekdayFUNC,
                                           node);
}
inline TableExprNode cdow (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::cdowFUNC, node);
}
inline TableExprNode ctodt (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::ctodFUNC, node);
}
inline TableExprNode cdate (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::cdateFUNC, node);
}
inline TableExprNode ctime (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::ctimeFUNC, node);
}
inline TableExprNode hms (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::hmsFUNC, node);
}
inline TableExprNode dms (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::dmsFUNC, node);
}
inline TableExprNode hdms (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::hdmsFUNC, node);
}
inline TableExprNode toString (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::stringFUNC,
                                           node);
}
inline TableExprNode toString (const TableExprNode& node,
                               const TableExprNode& format)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::stringFUNC,
                                           node, format);
}
inline TableExprNode week (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::weekFUNC, node);
}
inline TableExprNode time (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::timeFUNC, node);
}
inline TableExprNode trim (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::trimFUNC, node);
}
inline TableExprNode ltrim (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::ltrimFUNC, node);
}
inline TableExprNode rtrim (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::rtrimFUNC, node);
}
inline TableExprNode substr (const TableExprNode& node,
                             const TableExprNode& pos)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::substrFUNC,
                                           node, pos);
}
inline TableExprNode substr (const TableExprNode& node,
                             const TableExprNode& pos,
                             const TableExprNode& npos)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::substrFUNC,
                                           node, pos, npos);
}
inline TableExprNode replace (const TableExprNode& node,
                              const TableExprNode& patt)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::replaceFUNC,
                                           node, patt);
}
inline TableExprNode replace (const TableExprNode& node,
                              const TableExprNode& patt,
                              const TableExprNode& repl)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::replaceFUNC,
                                           node, patt, repl);
}
inline TableExprNode isNaN (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::isnanFUNC, node);
}
inline TableExprNode isInf (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::isinfFUNC, node);
}
inline TableExprNode isFinite (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::isfiniteFUNC,
                                           node);
}
inline TableExprNode min (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrminFUNC,
                                           node);
}
inline TableExprNode max (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmaxFUNC,
                                           node);
}
inline TableExprNode sum (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrsumFUNC,
                                           node);
}
inline TableExprNode product (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrproductFUNC,
                                           node);
}
inline TableExprNode sumSquare (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrsumsqrFUNC,
                                           node);
}
inline TableExprNode mean (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmeanFUNC,
                                           node);
}
inline TableExprNode variance (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrvariance1FUNC,
                                           node);
}
inline TableExprNode stddev (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrstddev1FUNC,
                                           node);
}
inline TableExprNode avdev (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arravdevFUNC,
                                           node);
}
inline TableExprNode rms (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrrmsFUNC,
                                           node);
}
inline TableExprNode median (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmedianFUNC,
                                           node);
}
inline TableExprNode fractile (const TableExprNode& node,
                               const TableExprNode& fraction)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrfractileFUNC,
                                           node, fraction);
}
inline TableExprNode any (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arranyFUNC, node);
}
inline TableExprNode all (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrallFUNC, node);
}
inline TableExprNode ntrue (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrntrueFUNC, node);
}
inline TableExprNode nfalse (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrnfalseFUNC, node);
}
inline TableExprNode sums (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrsumsFUNC,
                                           array, axes);
}
inline TableExprNode products (const TableExprNode& array,
                               const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrproductsFUNC,
                                           array, axes);
}
inline TableExprNode sumSquares (const TableExprNode& array,
                                 const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrsumsqrsFUNC,
                                           array, axes);
}
inline TableExprNode mins (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrminsFUNC,
                                           array, axes);
}
inline TableExprNode maxs (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmaxsFUNC,
                                           array, axes);
}
inline TableExprNode means (const TableExprNode& array,
                            const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmeansFUNC,
                                           array, axes);
}
inline TableExprNode variances (const TableExprNode& array,
                                const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrvariances1FUNC,
                                           array, axes);
}
inline TableExprNode stddevs (const TableExprNode& array,
                              const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrstddevs1FUNC,
                                           array, axes);
}
inline TableExprNode avdevs (const TableExprNode& array,
                             const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arravdevsFUNC,
                                           array, axes);
}
inline TableExprNode rmss (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrrmssFUNC,
                                           array, axes);
}
inline TableExprNode medians (const TableExprNode& array,
                              const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmediansFUNC,
                                           array, axes);
}
inline TableExprNode fractiles (const TableExprNode& array,
                                const TableExprNode& fraction,
                                const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrfractilesFUNC,
                                           array, fraction, axes);
}
inline TableExprNode anys (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arranysFUNC,
                                           array, axes);
}
inline TableExprNode alls (const TableExprNode& array,
                           const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrallsFUNC,
                                           array, axes);
}
inline TableExprNode ntrues (const TableExprNode& array,
                             const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrntruesFUNC,
                                           array, axes);
}
inline TableExprNode nfalses (const TableExprNode& array,
                              const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrnfalsesFUNC,
                                           array, axes);
}
inline TableExprNode runningMin (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runminFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningMax (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runmaxFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningMean (const TableExprNode& node,
                                  const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runmeanFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningVariance (const TableExprNode& node,
                                      const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runvariance1FUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningStddev (const TableExprNode& node,
                                    const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runstddev1FUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningAvdev (const TableExprNode& node,
                                   const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runavdevFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningRms (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runrmsFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningMedian (const TableExprNode& node,
                                    const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runmedianFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningAny (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runanyFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode runningAll (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::runallFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedMin (const TableExprNode& node,
                               const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxminFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedMax (const TableExprNode& node,
                               const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxmaxFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedMean (const TableExprNode& node,
                                const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxmeanFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedVariance (const TableExprNode& node,
                                    const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxvariance1FUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedStddev (const TableExprNode& node,
                                  const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxstddev1FUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedAvdev (const TableExprNode& node,
                                 const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxavdevFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedRms (const TableExprNode& node,
                               const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxrmsFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedMedian (const TableExprNode& node,
                                  const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxmedianFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedAny (const TableExprNode& node,
                               const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxanyFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode boxedAll (const TableExprNode& node,
                               const TableExprNodeSet& halfBoxWidth)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::boxallFUNC,
                                           node, halfBoxWidth);
}
inline TableExprNode array (const TableExprNode& values,
                            const TableExprNodeSet& shape)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrayFUNC,
                                           values, shape);
}
inline TableExprNode marray (const TableExprNode& array,
                             const TableExprNode& mask)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::marrayFUNC,
                                           array, mask);
}
inline TableExprNode arrayData (const TableExprNode& array)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrdataFUNC,
                                           array);
}
inline TableExprNode arrayMask (const TableExprNode& array)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrmaskFUNC,
                                           array);
}
inline TableExprNode arrayFlatten (const TableExprNode& array)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::arrflatFUNC,
                                           array);
}
inline TableExprNode transpose (const TableExprNode& array)
{
    // Needs an empty axes argument.
    return TableExprNode::newFunctionNode (TableExprFuncNode::transposeFUNC,
                                           array,
                                           TableExprNode(Vector<Int>()));
}
inline TableExprNode transpose (const TableExprNode& array,
                                const TableExprNodeSet& axes)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::transposeFUNC,
                                           array, axes);
}
inline TableExprNode diagonal (const TableExprNode& array)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::diagonalFUNC,
                                           array,
                                           TableExprNode(Vector<Int>()));
}
inline TableExprNode isdefined (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::isdefFUNC, node);
}
inline TableExprNode nelements (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::nelemFUNC, node);
}
inline TableExprNode ndim (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::ndimFUNC, node);
}
inline TableExprNode shape (const TableExprNode& node)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::shapeFUNC, node);
}
inline TableExprNode iif (const TableExprNode& condition,
                          const TableExprNode& arg1,
                          const TableExprNode& arg2)
{
    return TableExprNode::newFunctionNode (TableExprFuncNode::iifFUNC,
                                           condition, arg1, arg2);
}



} //# NAMESPACE CASACORE - END

#endif
