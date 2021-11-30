#include "modules.h"

int findEndModule()
{
    struct multiboot_tag *tag;
    void * module;
    unsigned long addr = (unsigned long)multiboot_ptr+KERNEL_OFFSET;
    unsigned int size = *(unsigned *)addr;

    for (tag = (struct multiboot_tag *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_MODULE:
                    module = (void *)(((struct multiboot_tag_module *) tag)->mod_end);
                    break;
            default:
                    break;
        }
    }
    return module;
}
int countModules()
{
    struct multiboot_tag *tag;
    int modules;
    unsigned long addr = (unsigned long)multiboot_ptr+KERNEL_OFFSET;
    unsigned int size = *(unsigned *)addr;

    for (tag = (struct multiboot_tag *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_MODULE:
                    modules++;
                    break;
            default:
                    break;
        }
    }
}

void callModule(void * module)
{
    typedef void (*call_module_t)(void);
    /* ... */
    call_module_t start_program = (call_module_t) (module+KERNEL_OFFSET);

    start_program();
}