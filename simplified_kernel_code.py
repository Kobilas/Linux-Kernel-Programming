MAX_ORDER = 5
# /mm/page_alloc.c
def __free_one_page(pfn:int, order:int):
    page_idx = None
    combined_idx = None
    buddy_idx = None
    max_order = MAX_ORDER
    page_idx = pfn & ((1 << max_order) - 1)
    i = 0
    print("   i|order|max_order|page_idx|buddy_idx|combined_idx")
    print("iter: ", i, order, max_order, page_idx, buddy_idx, combined_idx)
    i += 1
    while order < max_order - 1:
        print("iter: ", i, order, max_order, page_idx, buddy_idx, combined_idx)
        buddy_idx = __find_buddy_pfn(page_idx, order)
        combined_idx = buddy_idx & page_idx
        page_idx = combined_idx
        order += 1
        i += 1
    print("final:", i, order, max_order, page_idx, buddy_idx, combined_idx)

# /mm/internal.h
def __find_buddy_pfn(page_pfn:int, order:int):
    return page_pfn ^ (1 << order)

__free_one_page(25, 0)