//
//  script.cpp
//  blocksci_devel
//
//  Created by Harry Kalodner on 3/6/17.
//  Copyright © 2017 Harry Kalodner. All rights reserved.
//

#define BLOCKSCI_WITHOUT_SINGLETON

#include "util.hpp"
#include "script.hpp"

#include "pubkey_script.hpp"
#include "multisig_script.hpp"
#include "scripthash_script.hpp"
#include "nulldata_script.hpp"
#include "nonstandard_script.hpp"

#include "address/address_info.hpp"
#include "address/address_index.hpp"

#include "chain/transaction.hpp"

#include <iostream>


namespace blocksci {
    
    
    void Script::visitPointers(const std::function<void(const Address &)> &) const {}
    
    template<AddressType::Enum type>
    struct ScriptCreateFunctor {
        static std::unique_ptr<Script> f(const ScriptAccess &access, const Address &address) {
            constexpr auto scriptType = AddressInfo<type>::scriptType;
            return std::make_unique<ScriptAddress<scriptType>>(access, address.addressNum);
        }
    };
    
    std::unique_ptr<Script> Script::create(const ScriptAccess &access, const Address &address) {
        static constexpr auto table = make_dynamic_table<AddressType, ScriptCreateFunctor>();
        static constexpr std::size_t size = AddressType::all.size();
        
        auto index = static_cast<size_t>(address.type);
        if (index >= size)
        {
            throw std::invalid_argument("combination of enum values is not valid");
        }
        return table[index](access, address);
    }
    
    std::vector<const Output *> Script::getOutputs(const AddressIndex &index, const ChainAccess &chain) const {
        return index.getOutputs(*this, chain);
    }
    
    std::vector<const Input *> Script::getInputs(const AddressIndex &index, const ChainAccess &chain) const {
        return index.getInputs(*this, chain);
    }
    
    std::vector<Transaction> Script::getTransactions(const AddressIndex &index, const ChainAccess &chain) const {
        return index.getTransactions(*this, chain);
    }
    
    std::vector<Transaction> Script::getOutputTransactions(const AddressIndex &index, const ChainAccess &chain) const {
        return index.getOutputTransactions(*this, chain);
    }
    
    std::vector<Transaction> Script::getInputTransactions(const AddressIndex &index, const ChainAccess &chain) const {
        return index.getInputTransactions(*this, chain);
    }

}
