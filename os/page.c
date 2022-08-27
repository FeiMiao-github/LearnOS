#include <stdint.h>
#include <stddef.h>

#define PAEG_BIT_OFFSET    (12)
#define PAGE_SIZE          (4096)
#define RESERVED_PAGE_NUM  (2 << 4)
#define REVERSED_PAGE_SIZE (PAGE_SIZE << 4)

#define PAGE_USED (1)
#define PAGE_END  (1 << 1)

#define PAGE_NR_TO_PAGE_ADDR(nr) ((void*)(alloc_start + ((nr) << 12)))
#define PAGE_ADDR_TO_PAGE_NR(addr) ((uint64_t)((addr - alloc_start) >> 12))
#define PAGE_NR_CFG_ADDR(nr) (((Page_t*)HEAP_START) + nr)

#define IS_USED(index) (((Page_t*)HEAP_START)[index].flags & PAGE_USED)
#define IS_END(index) (((Page_t*)HEAP_START)[index].flags & PAGE_END)

extern uint64_t _memory_start;
extern uint64_t _memory_end;
extern uint64_t _heap_start;
extern uint64_t _heap_size;
extern uint64_t _bss_start;
extern uint64_t _bss_end;
extern uint64_t etext;

void printf(const char* fmt, ...);

typedef struct {
    uint8_t flags;
} Page_t;

static uint8_t* MEM_START = (uint8_t*)&_memory_start;
static uint8_t* MEM_END = (uint8_t*)&_memory_end;
static uint8_t* HEAP_START = (uint8_t*)&_heap_start;
static uint64_t HEAP_SIZE = (uint64_t)&_heap_size;

static uint64_t num_of_page = 0;
static void* alloc_start = NULL;

void clear_bss()
{
    for (uint64_t* start = &_bss_start; start < &_bss_end; start++)
    {
        *start = 0;
    }
}

int page_init()
{
    printf("MEM_START     = 0x%lx\n", MEM_START);
    printf("MEM_END       = 0x%lx\n", MEM_END);
    printf("HEAP_START    = 0x%lx\n", HEAP_START);
    printf("HEAP_SIZE     = 0x%lx\n", HEAP_SIZE);
    // printf("_bss_start    = 0x%lx\n", &_bss_start);
    // printf("_bss_end      = 0x%lx\n", (uint64_t)&_bss_end);

    num_of_page = (HEAP_SIZE / PAGE_SIZE) - RESERVED_PAGE_NUM;
    printf("num_of_page   = %d\n", num_of_page);

    Page_t* page = (Page_t*)HEAP_START;
    for (uint64_t i = 0; i < num_of_page; i++) {
        page[i].flags = 0;
    }

    printf("PageStart = 0x%lx\n", page);
    printf("PageEnd   = 0x%lx\n", page + (num_of_page + 8) / 8 * 8);

    uint64_t address = (uint64_t)HEAP_START + REVERSED_PAGE_SIZE;
    alloc_start = (void*)((address - 1 + PAGE_SIZE) & ~(PAGE_SIZE - 1));
    void* alloc_end = alloc_start + (num_of_page << PAEG_BIT_OFFSET);
    printf("address       = 0x%x\n", address);
    printf("alloc_start   = 0x%x\n", alloc_start);
    printf("alloc_end     = 0x%x\n", alloc_end);

    return 0;
}

static inline void set_used_flag(Page_t* start, uint64_t num)
{
    for (uint64_t i = 0; i < num; i++)
    {
        (start + i)->flags |= PAGE_USED;
    }
}

static inline void set_end(Page_t* page)
{
    page->flags |= PAGE_END;
}

static inline void unset_page_flag(Page_t* page)
{
    while (!(page-> flags & PAGE_END))
    {
        page->flags = 0;
        page++;
    }
}

void * find_first_cont_free_page(uint64_t num)
{
    void* ret = NULL;
    for (uint64_t i = 0, j = 0; i < num_of_page; i++)
    {
        if (IS_USED(i))
        {
            ret = NULL;
            j = 0;
        }
        else if (ret == NULL)
        {
            ret = PAGE_NR_TO_PAGE_ADDR(i);
            j = 1;
        }
        else
        {
            j++;
        }

        if (j == num)
        {
            break;
        }
    }
    return ret;
}

void * page_alloc(uint64_t num)
{
    if (num == 0)
    {
        return NULL;
    }
    
    void* ret = find_first_cont_free_page(num);
    if (ret == NULL)
    {
        return NULL;
    }

    uint64_t nr_start = PAGE_ADDR_TO_PAGE_NR(ret);
    Page_t* cfg = PAGE_NR_CFG_ADDR(nr_start);
    set_used_flag(cfg, num);
    set_end(cfg + num - 1);

    return ret;
}

void page_free(void* addr)
{
    uint64_t nr = PAGE_ADDR_TO_PAGE_NR(addr);
    Page_t* cfg = PAGE_NR_CFG_ADDR(nr);
    unset_page_flag(cfg);
}

#ifdef PAGE_TEST
void page_alloc_test()
{
    printf("[PAGE] page_alloc_test run !\n");
    void* addr1 = page_alloc(10);
    void* addr2 = page_alloc(5);
    printf("[PAGE] alloc addr1: %p addr2: %p\n", addr1, addr2);
    page_free(addr1);

    void* addr3 = page_alloc(5);
    addr1 = page_alloc(5);
    printf("[PAGE] alloc add1: %p addr2: %p addr3: %p\n", addr1, addr2, addr3);

    page_free(addr1);
    page_free(addr2);
    page_free(addr3);

    addr1 = page_alloc(2);
    addr2 = page_alloc(2);
    addr3 = page_alloc(2);
    printf("[PAGE] alloc add1: %p addr2: %p addr3: %p\n", addr1, addr2, addr3);
    page_free(addr1);
    page_free(addr2);
    page_free(addr3);
}
#endif // PAGE_TEST