/*
*   与hash_set 基本相同，底层以 hash_table 完成
*   使用的是 insert_unique，不允许键值重复？
*/
#ifndef __SGI_STL_HASH_MAP_H
#define __SGI_STL_HASH_MAP_H

#ifndef __SGI_STL_INTERNAL_HASHTABLE_H
#include <stl_hashtable.h>
#endif 

#include <stl_hash_map.h>

#ifdef __STL_USE_NAMESPACES
using __STD::hash;
using __STD::hashtable;
using __STD::hash_map;
using __STD::hash_multimap;
#endif /* __STL_USE_NAMESPACES */


#endif /* __SGI_STL_HASH_MAP_H */

// Local Variables:
// mode:C++
// End:
