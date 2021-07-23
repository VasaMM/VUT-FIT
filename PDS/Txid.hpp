/**
 * @file Txid.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (09:47)
 * @brief Definice typu Txid
 */

#ifndef TXID_HPP
#define TXID_HPP

#include "bencode.hpp"

/// Datový typ \c Txid
typedef bencode::integer Txid;

/// Konstanta pro zprávu bez \c Txid
/// Vetšinou značí chybu
const Txid NO_ID = -1;

#endif