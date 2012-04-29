
/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
/*
 *  Copyright (C) 2007 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef NodeInfo_h
#define NodeInfo_h

#include "Nodes.h"
#include "Parser.h"

namespace AJ {

    template <typename T> struct NodeInfo {
        T m_node;
        CodeFeatures m_features;
        int m_numConstants;
    };

    typedef NodeInfo<FuncDeclNode*> FuncDeclNodeInfo;    
    typedef NodeInfo<FuncExprNode*> FuncExprNodeInfo;
    typedef NodeInfo<ExpressionNode*> ExpressionNodeInfo;
    typedef NodeInfo<ArgumentsNode*> ArgumentsNodeInfo;
    typedef NodeInfo<ConstDeclNode*> ConstDeclNodeInfo;
    typedef NodeInfo<PropertyNode*> PropertyNodeInfo;
    typedef NodeInfo<PropertyList> PropertyListInfo;
    typedef NodeInfo<ElementList> ElementListInfo;
    typedef NodeInfo<ArgumentList> ArgumentListInfo;
    
    template <typename T> struct NodeDeclarationInfo {
        T m_node;
        ParserArenaData<DeclarationStacks::VarStack>* m_varDeclarations;
        ParserArenaData<DeclarationStacks::FunctionStack>* m_funcDeclarations;
        CodeFeatures m_features;
        int m_numConstants;
    };
    
    typedef NodeDeclarationInfo<StatementNode*> StatementNodeInfo;
    typedef NodeDeclarationInfo<CaseBlockNode*> CaseBlockNodeInfo;
    typedef NodeDeclarationInfo<CaseClauseNode*> CaseClauseNodeInfo;
    typedef NodeDeclarationInfo<SourceElements*> SourceElementsInfo;
    typedef NodeDeclarationInfo<ClauseList> ClauseListInfo;
    typedef NodeDeclarationInfo<ExpressionNode*> VarDeclListInfo;
    typedef NodeDeclarationInfo<ConstDeclList> ConstDeclListInfo;
    typedef NodeDeclarationInfo<ParameterList> ParameterListInfo;

} // namespace AJ

#endif // NodeInfo_h
