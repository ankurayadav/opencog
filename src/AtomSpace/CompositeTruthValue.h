/*
 * src/AtomSpace/CompositeTruthValue.h
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Welter Silva <welter@vettalabs.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _COMPOSITE_TRUTH_VALUE_H_
#define _COMPOSITE_TRUTH_VALUE_H_

#include "TruthValue.h"
#include "HandleMap.h"
#include "VersionHandle.h"

namespace opencog
{

typedef opencog::hash_map<VersionHandle, TruthValue*, hashVersionHandle, eqVersionHandle> VersionedTruthValueMap;

class CompositeTruthValue: public TruthValue
{

private:
    TruthValue* primaryTV;
    VersionedTruthValueMap versionedTVs;
    CompositeTruthValue(); // special constructor for being used inside fromString() method.

protected:
    const TruthValue& getPrimaryTV();
    void init(const TruthValue&, VersionHandle);
    void clear();
    void copy(const CompositeTruthValue&);

public:

    /**
     * @param The initial primary or versioned TV of this composite TV. If it is NULL_TV(), a
     *        default tv will be created internally.
     * @param The VersionHandle the passed TV is associated to. For primary TV, its substantive
     *        component must be UNDEFINED_HANDLE (you can use NULL_VERSION_HANDLE constant).
     *        In this case its indicator component does not matter.
     * NOTE: This object will take care of memory deallocation of the TruthValue object
     * passed as argument to this method. So, caller should not delete it outside.
     */
    CompositeTruthValue(const TruthValue&, VersionHandle);
    CompositeTruthValue(CompositeTruthValue const&);
    ~CompositeTruthValue();

    CompositeTruthValue* clone() const;
    TruthValue* merge(const TruthValue&) const;

    CompositeTruthValue& operator=(const TruthValue& rhs) throw (RuntimeException);

    // The following operator method was created because when a tv is
    // assigned to a variable declared as CompositeTruthValue, it
    // does not match the operator method above (that receives a
    // "const TruthValue&" argument).
    // Strangely, this does not happen with other TruthValue
    // subclasses (Simple and Indefinite, for instance...)
    CompositeTruthValue& operator=(const CompositeTruthValue& rhs) throw (RuntimeException);

    virtual bool operator==(const TruthValue& rhs) const;

    static CompositeTruthValue* fromString(const char*) throw (InvalidParamException);

    float getMean() const;
    float getCount() const;
    float getConfidence() const;

    float toFloat() const;
    std::string toString() const;
    TruthValueType getType() const;

    /*
     * Sets the primary or a versioned TV in this CTV object.
     * @param TruthValue object to be set. If it is a NULL_TV(), a default tv will be created
     *        internally.
     * @param The VersionHandle the passed TV is associated to. For primary TV, its substantive
     *        component must be UNDEFINED_HANDLE (you can use NULL_VERSION_HANDLE constant).
     *        In this case its indicator component does not matter.
     * NOTE: This object will take care of memory deallocation of the TruthValue object
     * passed as argument to this method. So, caller should not delete it outside.
     */
    void setVersionedTV(const TruthValue&, VersionHandle);

    /**
     * Gets the versioned TruthValue object associated with the given VersionHandle.
     * If NULL_VERSION_HANDLE is given as argument, returns the primaryTV.
     */
    const TruthValue& getVersionedTV(VersionHandle) const;

    /**
     * Removes the versioned TruthValue object associated with the given VersionHandle
     */
    void removeVersionedTV(VersionHandle);

    /**
     * Removes all versioned TruthValue objects associated with any VersionHandle whose
     * substantive component is equals to the given Handle
     */
    void removeVersionedTVs(Handle);

    /**
     * Gets the number of versioned TVs of this CTV.
     */
    int getNumberOfVersionedTVs() const;

    /**
     * Gets the versioned TV given its index in the internal map.
     * @param Index of the versioned TV. It must be a number between 0 and
     * (getNumberOfVersionedTVs()-1). Otherwise, it returns a NULL_VERSION_HANDLE.
     */
    VersionHandle getVersionHandle(int) const;

    /**
     * Updates all VersionHandles of the versioned TVs in this object using the
     * HandleMap passed as argument.
     * @param A HandleMap that maps old Handles to new ones.
     */
    void updateVersionHandles(HandleMap<Atom *> *handles);
};

} // namespace opencog

#endif
