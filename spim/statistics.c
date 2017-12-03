//Added 4/26/03 for performance measuring with Cool
//based on code by (I think) Douglas Hauge.
//Copied 8/21/06 by MMS.  I got a copy of this code from Francis Hsu.
//I am making it work with spim-7.2.1.

#include "statistics.h"

/* Do some dirty things so we can bolt on a quick size calculator */
typedef unsigned int  uint32;
typedef uint32 /*@alt int @*/ mem_addr;
extern void user_kernel_text_segment(bool);
extern mem_addr find_symbol_address(char*);
extern mem_addr current_text_pc();
#ifndef END_OF_TRAP_HANDLER_SYMBOL
#define END_OF_TRAP_HANDLER_SYMBOL "__eoth"
#endif
/* End of the filth */


void statistics_reset(statistics_t *s)
{
  memset(s, 0, sizeof(s));
}

void statistics_print(statistics_t *s,FILE *f)
{
  mem_addr dump_start;
  mem_addr dump_end;

  /* dump text segment */
  user_kernel_text_segment (false);
  dump_start = find_symbol_address (END_OF_TRAP_HANDLER_SYMBOL);
  dump_end = current_text_pc ();
    
  int other_instructions =  
    s->instruction_count - (s->num_reads + s->num_writes + s->num_branches);
  fprintf(f,
	  "Stats -- #instructions : %d\n"
	  "         #reads : %d  #writes %d  #branches %d  #other %d  static size: %d\n",
	  s->instruction_count,
	  s->num_reads,s->num_writes,s->num_branches, other_instructions,
      dump_end - dump_start
	  );
}

void statistics_inc_instruction_count(statistics_t *s)
{
   s->instruction_count += 1;
}

void statistics_inc_reads(statistics_t *s)
{
   s->num_reads += 1;
}

void statistics_inc_writes(statistics_t *s)
{
   s->num_writes += 1;
}

void statistics_inc_branches(statistics_t *s)
{
   s->num_branches += 1;
}

statistics_t global_stats;
int keep_stats = 0;
