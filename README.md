# 🧠 Linux Page Table Walker — Virtual Memory Address Translator

This project implements a Linux kernel module that walks the page tables of a given **PID** and **virtual address**, determining whether the page is:

- Mapped to physical memory  
- Swapped out  
- Or not present at all  

It prints the physical address or swap identifier directly from kernel space, demonstrating understanding of **Linux virtual memory**, **page table traversal**, and **kernel programming**.

---

## 🚀 Features

### 🔍 Page Table Traversal
The module manually walks the following page table levels:

- PGD  
- P4D  
- PUD  
- PMD  
- PTE  

It returns the **PTE entry** if found, or logs that the mapping does not exist.

### 📦 Physical Address Resolution
If the page is present (`pte_present()`):

- Extracts PFN (`pte_pfn()`)  
- Computes physical address:  
# 🧠 Linux Page Table Walker — Virtual Memory Address Translator

This project implements a Linux kernel module that walks the page tables of a given **PID** and **virtual address**, determining whether the page is:

- Mapped to physical memory  
- Swapped out  
- Or not present at all  

It prints the physical address or swap identifier directly from kernel space, demonstrating understanding of **Linux virtual memory**, **page table traversal**, and **kernel programming**.

---

## 🚀 Features

### 🔍 Page Table Traversal
The module manually walks the following page table levels:

- PGD  
- P4D  
- PUD  
- PMD  
- PTE  

It returns the **PTE entry** if found, or logs that the mapping does not exist.

### 📦 Physical Address Resolution
If the page is present (`pte_present()`):

- Extracts PFN (`pte_pfn()`)  
- Computes physical address:  
phys = (pfn << PAGE_SHIFT) | (addr & (PAGE_SIZE - 1))

### 💾 Swap Identification
If the page is *not present*:

- Extracts swap entry using `pte_to_swp_entry()`  
- Logs swap identifier
