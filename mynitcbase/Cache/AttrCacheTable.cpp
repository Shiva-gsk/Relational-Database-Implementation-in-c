#include "AttrCacheTable.h"

#include <cstring>

AttrCacheEntry* AttrCacheTable::attrCache[MAX_OPEN];

/* returns the attrOffset-th attribute for the relation corresponding to relId
NOTE: this function expects the caller to allocate memory for `*attrCatBuf`
*/
int AttrCacheTable::getAttrCatEntry(int relId, int attrOffset, AttrCatEntry* attrCatBuf) {
    // check if 0 <= relId < MAX_OPEN and return E_OUTOFBOUND otherwise
    if (relId < 0 || relId >= MAX_OPEN) {
        return E_OUTOFBOUND;
    }

    // check if attrCache[relId] == nullptr and return E_RELNOTOPEN if true
    if (attrCache[relId] == nullptr) {
        return E_RELNOTOPEN;
    }

    // traverse the linked list of attribute cache entries
    for (AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) {
        if (entry->attrCatEntry.offset == attrOffset) {
            // copy entry->attrCatEntry to *attrCatBuf and return SUCCESS;
            *attrCatBuf = entry->attrCatEntry;
            return SUCCESS;
        }
    }

    // there is no attribute at this offset
    return E_ATTRNOTEXIST;
}

/* returns the attribute with name `attrName` for the relation corresponding to relId
NOTE: this function expects the caller to allocate memory for `*attrCatBuf`
*/
int AttrCacheTable::getAttrCatEntry(int relId, char attrName[ATTR_SIZE], AttrCatEntry* attrCatBuf) {
    // check that relId is valid and corresponds to an open relation
    if (relId < 0 || relId >= MAX_OPEN) {
        return E_OUTOFBOUND;
    }
    if (attrCache[relId] == nullptr) {
        return E_RELNOTOPEN; // check if relation is open
    }

    // iterate over the entries in the attribute cache and set attrCatBuf to the entry that
    //    matches attrName
    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) {
        if(strcmp(entry->attrCatEntry.attrName, attrName) == 0) {
            *attrCatBuf = entry->attrCatEntry;
            return SUCCESS;
        }
    }

    // no attribute with name attrName for the relation
    return E_ATTRNOTEXIST;
}



/* Converts a attribute catalog record to AttrCatEntry struct
    We get the record as Attribute[] from the BlockBuffer.getRecord() function.
    This function will convert that to a struct AttrCatEntry type.
*/
void AttrCacheTable::recordToAttrCatEntry(union Attribute record[ATTRCAT_NO_ATTRS], AttrCatEntry* attrCatEntry) {
    // copy values from record to attrCatEntry fields
    strcpy(attrCatEntry->relName, record[ATTRCAT_REL_NAME_INDEX].sVal); // Relation Name
    strcpy(attrCatEntry->attrName, record[ATTRCAT_ATTR_NAME_INDEX].sVal); // Attribute Name
    attrCatEntry->offset = (int)record[ATTRCAT_OFFSET_INDEX].nVal; // Offset
    attrCatEntry->attrType = (int)record[ATTRCAT_ATTR_TYPE_INDEX].nVal; // Attribute Type
    attrCatEntry->rootBlock = (int)record[ATTRCAT_ROOT_BLOCK_INDEX].nVal; // Root Block
    attrCatEntry->primaryFlag = (bool)record[ATTRCAT_PRIMARY_FLAG_INDEX].nVal; // Primary Flag
}

void AttrCacheTable::attrCatEntryToRecord(AttrCatEntry *attrCatEntry, union Attribute record[ATTRCAT_NO_ATTRS]){
    // copy values from attrCatEntry fields to record
    strcpy(record[ATTRCAT_REL_NAME_INDEX].sVal, attrCatEntry->relName); // Relation Name
    strcpy(record[ATTRCAT_ATTR_NAME_INDEX].sVal, attrCatEntry->attrName); // Attribute Name
    record[ATTRCAT_OFFSET_INDEX].nVal = attrCatEntry->offset; // Offset
    record[ATTRCAT_ATTR_TYPE_INDEX].nVal = attrCatEntry->attrType; // Attribute Type
    record[ATTRCAT_ROOT_BLOCK_INDEX].nVal = attrCatEntry->rootBlock; // Root Block
    record[ATTRCAT_PRIMARY_FLAG_INDEX].nVal = attrCatEntry->primaryFlag; // Primary Flag
}

int AttrCacheTable::getSearchIndex(int relId, char attrName[ATTR_SIZE], IndexId *searchIndex) {

    if(relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]*/
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ // entry corresponding to the relId in the Attribute Cache Table is free*/
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) { // each attribute corresponding to relation with relId
        if (strcmp(entry->attrCatEntry.attrName, attrName) == 0){ // attrName field of the AttrCatEntryis equal to the input attrName 
        //copy the searchIndex field of the corresponding Attribute Cache entry
        //in the Attribute Cache Table to input searchIndex variable.
            *searchIndex = entry->searchIndex;
            return SUCCESS;
        }
    }

    return E_ATTRNOTEXIST;
}

int AttrCacheTable::getSearchIndex(int relId, int attrOffset, IndexId *searchIndex) {

    if(relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ // entry corresponding to the relId in the Attribute Cache Table is free
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) { // each attribute corresponding to relation with relId
        if (entry->attrCatEntry.offset == attrOffset) { // attrName/offset field of the AttrCatEntry is equal to the input attrOffset
        //copy the searchIndex field of the corresponding Attribute Cache entry
        //in the Attribute Cache Table to input searchIndex variable.
            *searchIndex = entry->searchIndex;
            return SUCCESS;
        }
    }

    return E_ATTRNOTEXIST;
}

int AttrCacheTable::setSearchIndex(int relId, char attrName[ATTR_SIZE], IndexId *searchIndex) {

    if(relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ // entry corresponding to the relId in the Attribute Cache Table is free
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) { // each attribute corresponding to relation with relId
        if (strcmp(entry->attrCatEntry.attrName, attrName) == 0) { // attrName field of the AttrCatEntry is equal to the input attrName
            // copy the input searchIndex variable to the searchIndex field of the
            //corresponding Attribute Cache entry in the Attribute Cache Table.
            entry->searchIndex = *searchIndex;
            return SUCCESS;
        }
    }

    return E_ATTRNOTEXIST;
}

int AttrCacheTable::setSearchIndex(int relId, int attrOffset, IndexId *searchIndex) {

    if(relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ // entry corresponding to the relId in the Attribute Cache Table is free
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) { // each attribute corresponding to relation with relId
        if (entry->attrCatEntry.offset == attrOffset) { // attrName/offset field of the AttrCatEntry is equal to the input attrOffset
            // copy the input searchIndex variable to the searchIndex field of the
            //corresponding Attribute Cache entry in the Attribute Cache Table.
            entry->searchIndex = *searchIndex;
            return SUCCESS;
        }
    }

    return E_ATTRNOTEXIST;
}

int AttrCacheTable::resetSearchIndex(int relId, char attrName[ATTR_SIZE]) {

    // declare an IndexId having value {-1, -1}
    IndexId invalidIndex = {-1, -1};
    // set the search index to {-1, -1} using AttrCacheTable::setSearchIndex
    // return the value returned by setSearchIndex
    return setSearchIndex(relId, attrName, &invalidIndex);
}

int AttrCacheTable::resetSearchIndex(int relId, int attrOffset) {

    // declare an IndexId having value {-1, -1}
    IndexId invalidIndex = {-1, -1};
    // set the search index to {-1, -1} using AttrCacheTable::setSearchIndex
    // return the value returned by setSearchIndex
    return setSearchIndex(relId, attrOffset, &invalidIndex);
}

int AttrCacheTable::setAttrCatEntry(int relId, char attrName[ATTR_SIZE], AttrCatEntry *attrCatBuf) {

    if (relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ //entry corresponding to the relId in the Attribute Cache Table is free
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) { // each attribute corresponding to relation with relId
        if (strcmp(entry->attrCatEntry.attrName, attrName) == 0) { // attrName field of the AttrCatEntry is equal to the input attrName
            // copy the attrCatBuf to the corresponding Attribute Catalog entry in
            // the Attribute Cache Table.
            entry->attrCatEntry = *attrCatBuf;

            // set the dirty flag of the corresponding Attribute Cache entry in the
            // Attribute Cache Table.
            entry->dirty = true;

            return SUCCESS;
        }
    }

    return E_ATTRNOTEXIST;
}

int AttrCacheTable::setAttrCatEntry(int relId, int attrOffset, AttrCatEntry *attrCatBuf) {
    if(relId < 0 || relId >= MAX_OPEN) { // relId is outside the range [0, MAX_OPEN-1]
        return E_OUTOFBOUND;
    }

    if(attrCache[relId] == nullptr){ //entry corresponding to the relId in the Attribute Cache Table is free
        return E_RELNOTOPEN;
    }

    for(AttrCacheEntry* entry = attrCache[relId]; entry != nullptr; entry = entry->next) {
        if (entry->attrCatEntry.offset == attrOffset) { // attrName/offset field of the AttrCatEntry is equal to the input attrOffset
            // copy the attrCatBuf to the corresponding Attribute Catalog entry in
            // the Attribute Cache Table.
            entry->attrCatEntry = *attrCatBuf;
            // set the dirty flag of the corresponding Attribute Cache entry in the
            // Attribute Cache Table.
            entry->dirty = true;

            return SUCCESS;
        }
    }
    return E_ATTRNOTEXIST;
}z