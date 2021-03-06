/*
 * This file is part of John the Ripper password cracker,
 * Copyright (c) 2013 by Solar Designer
 * Copyright (c) 2013-2014 by magnum
 * Copyright (c) 2014 by Sayantan Datta
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */

/*
 * Mask mode cracker.
 */

#ifndef _JOHN_MASK_H
#define _JOHN_MASK_H

#include "loader.h"

#define MAX_NUM_MASK_PLHDR 255 // Maximum number of placeholders in a mask.

typedef struct {
	/* store locations of op braces in mask */
	int stack_op_br[MAX_NUM_MASK_PLHDR + 1];
	/* store locations of cl braces in mask */
	int stack_cl_br[MAX_NUM_MASK_PLHDR + 1];
	/* store locations of valid ? in mask */
	int stack_qtn[MAX_NUM_MASK_PLHDR + 1];
	/* 1 if parse is successful, otherwise 0 */
	int parse_ok;
} parsed_ctx;

 /* Range of characters for a placeholder in the mask */
 /* Rearranging the structure could affect performance */
typedef struct {
	/* Characters in the range */
	unsigned char chars[0xFF];
	/* next active range */
	unsigned char next;
	/* current postion in chars[] while iterating */
	unsigned char iter;
	/* Number of characters in the range */
	unsigned char count;
	/*
	 * Set to zero when the characters in the range are not consecutive,
	 * otherwise start is set to the minumum value in range. Minimum
	 * value cannot be a null character which has a value zero.
	 */
	unsigned char start;
	/* Base postion of the characters in key */
	int pos;
	/* offset when a key is inserted from other mode */
	int offset;
} mask_range;

/* Processed mask structure for password generation on CPU */
typedef struct {
	/* Set of mask placeholders for generating password */
	mask_range ranges[MAX_NUM_MASK_PLHDR + 1];
	/* Positions in mask for iteration on CPU */
	int active_positions[MAX_NUM_MASK_PLHDR + 1];
	/* Postion of the first active range */
	int ps1;
	/* Total number of placeholders, cpu + gpu */
	int count;
	/* Number of placeholders active for iteration on CPU */
	int cpu_count;
	/* offset at which mask starts in the key */
	int offset;
} cpu_mask_context;

/*
 * Initialize mask mode cracker.
 */
extern void mask_init(struct db_main *db, char *unprocessed_mask);

/*
 * Runs the mask mode cracker.
 */
extern int do_mask_crack(const char *key);

extern void mask_done(void);

/*
 * These are exported for stacked modes (eg. hybrid mask)
 */
extern void mask_fix_state(void);
extern void mask_save_state(FILE *file);
extern int mask_restore_state(FILE *file);

/*
 * Total number of candidates to begin with. Remains unchanged throughout
 * one call to do_mask_crack but may vary with hybrid parent key length.
 */
extern unsigned long long mask_tot_cand;

/* Hybrid mask's contribution to key length. Eg. for bc?l?d?w this will be 4. */
extern int mask_add_len;

/* Number of ?w in hybrid mask */
extern int mask_num_qw;

/* Number of times parent mode called hybrid mask. */
extern unsigned long long mask_parent_keys;

/* Current length when pure mask mode iterates over length */
extern int mask_cur_len;

#endif
