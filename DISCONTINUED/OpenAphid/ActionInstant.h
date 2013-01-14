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

#ifndef OpenAphid_ActionInstant_h
#define OpenAphid_ActionInstant_h

#include "Action.h"
#include "AJCallback.h"
#include "LinkedList.h"

namespace Aphid {
	
	///-------------------------------------------------------------------------------------------------------------------
	class ShowAction : public InstantAction {
		typedef InstantAction Base;
		typedef ShowAction Self;
	public:
		virtual ~ShowAction();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "ShowAction";}
		
	protected:
		ShowAction();
		
		virtual Self* doCreateClone()
		{
			return new Self();
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class HideAction : public InstantAction {
		typedef InstantAction Base;
		typedef HideAction Self;
	public:
		virtual ~HideAction();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "HideAction";}
		
	protected:
		HideAction();
		
		virtual Self* doCreateClone()
		{
			return new Self();
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ToggleVisibleAction : public InstantAction {
		typedef InstantAction Base;
		typedef ToggleVisibleAction Self;
	public:
		virtual ~ToggleVisibleAction();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "ToggleVisibleAction";}
		
	protected:
		ToggleVisibleAction();
		
		virtual Self* doCreateClone()
		{
			return new Self();
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FlipXAction : public InstantAction {
		typedef InstantAction Base;
		typedef FlipXAction Self;
	public:
		virtual ~FlipXAction();
		
		static PassRefPtr<Self> create(bool flipX)
		{
			return adoptRef(new Self(flipX));
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "FlipXAction";}		
		
	protected:
		FlipXAction(bool flipX);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_flipX);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(!m_flipX);
		}
		
		bool m_flipX;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FlipYAction : public InstantAction {
		typedef InstantAction Base;
		typedef FlipYAction Self;
	public:
		virtual ~FlipYAction();
		
		static PassRefPtr<Self> create(bool flipY)
		{
			return adoptRef(new Self(flipY));
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "FlipYAction";}
		
	protected:
		FlipYAction(bool flipY);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_flipY);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(!m_flipY);
		}
		
		bool m_flipY;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class PlaceAction : public InstantAction {
		typedef InstantAction Base;
		typedef PlaceAction Self;
	public:
		virtual ~PlaceAction();
		
		static PassRefPtr<Self> create(const Point& p)
		{
			return adoptRef(new Self(p));
		}
		
		virtual void startWithTarget(Node* target);
		
		virtual const char* name() const {return "PlaceAction";}
		
	protected:
		PlaceAction(const Point& p);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_position);
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
		
		Point m_position;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class CallAJFunctionAction : public InstantAction, public _LinkElement<CallAJFunctionAction> {
		typedef InstantAction Base;
		typedef CallAJFunctionAction Self;
	public:
		virtual ~CallAJFunctionAction();
		
		static PassRefPtr<Self> create(AJ::AJObject* callback)
		{
			if (callback)
				return adoptRef(new Self(callback));
			return 0;
		}
		
		virtual int baseType() const {return ActionTypeCallFunction | Base::baseType();}
		virtual const char* name() const {return "CallFunctionAction";}
		
		virtual void becomeManaged();
		
		virtual void startWithTarget(Node* target);
		
		//TODO: should I override stop() too?
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		CallAJFunctionAction(AJ::AJObject* callback);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_function ? m_function->optObject() : 0);
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
		
		RefPtr<MappedAJObject> m_function;
	};
	
	//TODO: several subclasses of CallFuncAction
}

#endif
