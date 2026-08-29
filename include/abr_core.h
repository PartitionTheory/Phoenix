#ifndef ABR_CORE_H
#define ABR_CORE_H

/* Phoenix Structural Machine — Core Contract Layer
 *
 * This header defines the fundamental types and constants used across
 * the Phoenix system. It contains no logic and no algorithms.
 * It is the semantic root of the structural machine.
 */

/* Phoenix boolean type */
typedef int phoenix_bool;

/* Phoenix core integer type */
typedef long long phoenix_int64;

/* Phoenix core ID type */
typedef int phoenix_id;

/* Phoenix core value type */
typedef long long phoenix_value;

/* Phoenix truth values */
#define PHOENIX_TRUE  1
#define PHOENIX_FALSE 0

#endif /* ABR_CORE_H */

