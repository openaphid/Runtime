
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
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_HashCountedSet_h
#define WTF_HashCountedSet_h

#include "Assertions.h"
#include "FastAllocBase.h"
#include "HashMap.h"
#include "Vector.h"

namespace ATF {

    template<typename Value, typename HashFunctions = typename DefaultHash<Value>::Hash,
        typename Traits = HashTraits<Value> > class HashCountedSet : public FastAllocBase {
    private:
        typedef HashMap<Value, unsigned, HashFunctions, Traits> ImplType;
    public:
        typedef Value ValueType;
        typedef typename ImplType::iterator iterator;
        typedef typename ImplType::const_iterator const_iterator;
        
        HashCountedSet() {}
        
        int size() const;
        int capacity() const;
        bool isEmpty() const;
        
        // Iterators iterate over pairs of values and counts.
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        
        iterator find(const ValueType&);
        const_iterator find(const ValueType&) const;
        bool contains(const ValueType&) const;
        unsigned count(const ValueType&) const;

        // Increases the count if an equal value is already present
        // the return value is a pair of an interator to the new value's 
        // location, and a bool that is true if an new entry was added.
        std::pair<iterator, bool> add(const ValueType&);
        
        // Reduces the count of the value, and removes it if count
        // goes down to zero, returns true if the value is removed.
        bool remove(const ValueType&);
        bool remove(iterator);
 
        // Removes the value, regardless of its count.
        void removeAll(iterator);
        void removeAll(const ValueType&);

        // Clears the whole set.
        void clear();

    private:
        ImplType m_impl;
    };
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline int HashCountedSet<Value, HashFunctions, Traits>::size() const
    {
        return m_impl.size(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline int HashCountedSet<Value, HashFunctions, Traits>::capacity() const
    {
        return m_impl.capacity(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline bool HashCountedSet<Value, HashFunctions, Traits>::isEmpty() const
    {
        return size() == 0; 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::iterator HashCountedSet<Value, HashFunctions, Traits>::begin()
    {
        return m_impl.begin(); 
    }

    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::iterator HashCountedSet<Value, HashFunctions, Traits>::end()
    {
        return m_impl.end(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::const_iterator HashCountedSet<Value, HashFunctions, Traits>::begin() const
    {
        return m_impl.begin(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::const_iterator HashCountedSet<Value, HashFunctions, Traits>::end() const
    {
        return m_impl.end(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::iterator HashCountedSet<Value, HashFunctions, Traits>::find(const ValueType& value)
    {
        return m_impl.find(value); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline typename HashCountedSet<Value, HashFunctions, Traits>::const_iterator HashCountedSet<Value, HashFunctions, Traits>::find(const ValueType& value) const
    {
        return m_impl.find(value); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline bool HashCountedSet<Value, HashFunctions, Traits>::contains(const ValueType& value) const
    {
        return m_impl.contains(value); 
    }

    template<typename Value, typename HashFunctions, typename Traits>
    inline unsigned HashCountedSet<Value, HashFunctions, Traits>::count(const ValueType& value) const
    {
        return m_impl.get(value);
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline std::pair<typename HashCountedSet<Value, HashFunctions, Traits>::iterator, bool> HashCountedSet<Value, HashFunctions, Traits>::add(const ValueType &value)
    {
        pair<iterator, bool> result = m_impl.add(value, 0); 
        ++result.first->second;
        return result;
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline bool HashCountedSet<Value, HashFunctions, Traits>::remove(const ValueType& value)
    {
        return remove(find(value));
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline bool HashCountedSet<Value, HashFunctions, Traits>::remove(iterator it)
    {
        if (it == end())
            return false;

        unsigned oldVal = it->second;
        ASSERT(oldVal);
        unsigned newVal = oldVal - 1;
        if (newVal) {
            it->second = newVal;
            return false;
        }

        m_impl.remove(it);
        return true;
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline void HashCountedSet<Value, HashFunctions, Traits>::removeAll(const ValueType& value)
    {
        removeAll(find(value));
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline void HashCountedSet<Value, HashFunctions, Traits>::removeAll(iterator it)
    {
        if (it == end())
            return;

        m_impl.remove(it);
    }
    
    template<typename Value, typename HashFunctions, typename Traits>
    inline void HashCountedSet<Value, HashFunctions, Traits>::clear()
    {
        m_impl.clear(); 
    }
    
    template<typename Value, typename HashFunctions, typename Traits, typename VectorType>
    inline void copyToVector(const HashCountedSet<Value, HashFunctions, Traits>& collection, VectorType& vector)
    {
        typedef typename HashCountedSet<Value, HashFunctions, Traits>::const_iterator iterator;
        
        vector.resize(collection.size());
        
        iterator it = collection.begin();
        iterator end = collection.end();
        for (unsigned i = 0; it != end; ++it, ++i)
            vector[i] = *it;
    }

    template<typename Value, typename HashFunctions, typename Traits>
    inline void copyToVector(const HashCountedSet<Value, HashFunctions, Traits>& collection, Vector<Value>& vector)
    {
        typedef typename HashCountedSet<Value, HashFunctions, Traits>::const_iterator iterator;
        
        vector.resize(collection.size());
        
        iterator it = collection.begin();
        iterator end = collection.end();
        for (unsigned i = 0; it != end; ++it, ++i)
            vector[i] = (*it).first;
    }


} // namespace khtml

using ATF::HashCountedSet;

#endif /* WTF_HashCountedSet_h */
