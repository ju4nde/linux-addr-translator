#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/swap.h>
#include <linux/swapops.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("JUAN NUNEZ AIMA");

static int pid =0;
static unsigned long long addr= 0;
module_param(pid,int,0);
module_param(addr,ullong,0);

static pte_t *page_walk(struct mm_struct* mm){ //function that returns pte, returns null if not found
  pgd_t *pgd;
  p4d_t *p4d;
  pmd_t *pmd;
  pud_t *pud;
  pte_t *pte;
  pgd = pgd_offset(mm, addr);
  if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd))) return NULL;
  p4d = p4d_offset(pgd, addr);
  if (p4d_none(*p4d) || unlikely(p4d_bad(*p4d))) return NULL;
  pud = pud_offset(p4d, addr);
  if (pud_none(*pud) || unlikely(pud_bad(*pud))) return NULL;
  pmd = pmd_offset(pud, addr);
  if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd))) return NULL;
  pte = pte_offset_kernel(pmd,addr);
  if (!pte) return NULL;
  
  return pte;
}


static int __init memory_manager_init(void){
  pte_t *pte;
  struct mm_struct* mm;
  struct task_struct *ts;

  for_each_process(ts){ //get the process pointer by looking up the pid
    if(ts->pid == pid){ //once we found it we assign the tasks memory management to a pointer 
        mm = ts->mm; 
        break;
    }
  }
  pte= page_walk(mm); //we pass that memory management value to the page walking function
  if(pte == NULL) printk(KERN_INFO "[CSE330-Memory-Manager] PID [%d]: virtual address [%llx] physical address [NA] swap identifier [NA]\n", pid, addr); //if it returns null that means it wasnt found
  else 
  {
    if(pte_present(*pte)) // if returns 1 pte is in memory
    { 
      unsigned long pfn = pte_pfn(*pte); //we get the pfn with the pte
      unsigned long page_number = pfn << PAGE_SHIFT;
      unsigned long offset = addr & (PAGE_SIZE -1); 
      unsigned long phys_addr = page_number | offset;
      printk(KERN_INFO "[CSE330-Memory-Manager] PID [%d]: virtual address [%llx] physical address [%lx] swap identifier [NA]\n", pid, addr, phys_addr);
    }
    else //if it returns 0 its in swap
    {
    swp_entry_t entry = pte_to_swp_entry(*pte);
    unsigned long swap_id = entry.val;
    printk(KERN_INFO "[CSE330-Memory-Manager] PID [%d]: virtual address [%llx] physical address [NA] swap identifier [%lx]\n", pid, addr, swap_id);
    
    
    }
  
  }

  return 0;
 
}

static void __exit memory_manager_exit(void){
  
}


module_init(memory_manager_init);
module_exit(memory_manager_exit);



