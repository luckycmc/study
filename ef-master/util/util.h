// Copyright (c) 2018-2020 The EFramework Project
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _UTIL_HEADER_
#define _UTIL_HEADER_

#define CAST_PARENT_PTR(ptr, parent_type, field_name) \
((parent_type*)((char*)ptr-(char*)&((parent_type*)0)->field_name))

inline size_t ef_resize(size_t size, size_t min) __attribute__((always_inline));

inline size_t ef_resize(size_t size, size_t min)
{
    if (size < min) {
        size = min;
    }

    size -= 1;
    size |= (size >> 1);
    size |= (size >> 2);
    size |= (size >> 4);
    size |= (size >> 8);
    size |= (size >> 16);
#if __x86_64__
    size |= (size >> 32);
#endif

    return size + 1;
}

#endif
y) __attribute__((always_inline));
inline ef_list_entry_t *ef_list_entry_before(ef_list_entry_t *current) __attribute__((always_inline));
inline ef_list_entry_t *ef_list_entry_after(ef_list_entry_t *current) __attribute__((always_inline));
inline ef_list_entry_t *ef_list_remove_before(ef_list_entry_t *current) __attribute__((always_inline));
inline ef_list_entry_t *ef_list_remove_after(ef_list_entry_t *current) __attribute__((always_inline));

inline int ef_list_empty(ef_list_entry_t *head)
{
    return head->next == head;
}

inline void ef_list_init(ef_list_entry_t *head)
{
    head->prev = head;
    head->next = head;
}

inline void ef_list_remove(ef_list_entry_t *entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

inline void ef_list_insert_before(ef_list_entry_t *current, ef_list_entry_t *entry)
{
    entry->next = current;
    current->prev->next = entry;
    entry->prev = current->prev;
    current->prev = entry;
}

inline void ef_list_insert_after(ef_list_entry_t *current, ef_list_entry_t *entry)
{
    entry->next = current->next;
    current->next = entry;
    entry->prev = current;
    entry->next->prev = entry;
}

inline ef_list_entry_t *ef_list_entry_before(ef_list_entry_t *current)
{
    return current->prev;
}

inline ef_list_entry_t *ef_list_entry_after(ef_list_entry_t *current)
{
    return current->next;
}

inline ef_list_entry_t *ef_list_remove_before(ef_list_entry_t *current)
{
    ef_list_entry_t *entry;

    if (ef_list_empty(current)) {
        return NULL;
    }

    entry = current->prev;
    current->prev = entry->prev;
    current->prev->next = current;
    return entry;
}

inline ef_list_entry_t *ef_list_remove_after(ef_list_entry_t *current)
{
    ef_list_entry_t *entry;

    if (ef_list_empty(current)) {
        return NULL;
    }

    entry = current->next;
    current->next = entry->next;
    current->next->prev = current;
    return entry;
}

#endif
