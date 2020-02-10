/*
 * Copyright (C) 2018 GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Authors: Mathieu Barbe, GreenWaves Technologies (mathieu.barbe@greenwaves-technologies.com)
 */

<<<<<<< HEAD
#include "string.h"
#include "stdio.h"

=======
>>>>>>> 3.1.1_dev_001-edit_BitCraze_DD
#include "pmsis.h"
#include "bsp/bsp.h"
#include "bsp/flash.h"
#include "bsp/partition.h"
<<<<<<< HEAD
#include "flash_partition.h"

size_t pi_partition_get_size(const pi_partition_t *partition)
{
    if(partition)
=======

static pi_device_api_t partition_api = {
        .open = pi_partition_open,
        .close = pi_partition_close,
        .open_async = NULL,
        .close_async = NULL,
        .read = NULL,
        .write = NULL,
        .ioctl = NULL,
        .ioctl_async = NULL,
        .specific_api = NULL,
};

struct partition_table {
    uint32_t fs_offset;
    uint32_t reserved;
};

int pi_partition_open(struct pi_device *device)
{
    pi_partition_t *partition;
    struct partition_table *table;
    struct pi_partition_conf *conf = (struct pi_partition_conf *) device->config;

    // At this time, only two partitions are availables on the flash,
    if (conf->id >= 2) return -1;

    table = pi_l2_malloc(sizeof(*table));
    if (table == NULL) return -1;
    // Read partition 1 offset
    pi_flash_read(conf->flash, 0, table, 4);

    device->data = pi_l2_malloc(sizeof(pi_partition_t));
    if (device->data == NULL) goto table_error;
    partition = (pi_partition_t *) device->data;

    partition->flash = conf->flash;
    if (conf->id == 0)
    {
        partition->offset = 4;
        partition->size = table->fs_offset - partition->offset;
    } else
    {
        partition->offset = table->fs_offset;
        partition->size = (1 << 26) - partition->offset;
        // todo fetch flash size from flash ioctl.
    }

    device->api = &partition_api;

    pi_l2_free(table, sizeof(*table));
    return 0;

    table_error:
    pi_l2_free(table, sizeof(*table));
    return -1;
}

size_t pi_partition_get_size(pi_device_t *device)
{
    pi_partition_t *partition = (pi_partition_t *) device->data;
    if (partition)
>>>>>>> 3.1.1_dev_001-edit_BitCraze_DD
        return partition->size;
    else
        return 0;
}

<<<<<<< HEAD
uint32_t pi_partition_get_flash_offset(const pi_partition_t *partition)
{
    if(partition)
=======
uint32_t pi_partition_get_flash_offset(pi_device_t *device)
{
    pi_partition_t *partition = (pi_partition_t *) device->data;
    if (partition)
>>>>>>> 3.1.1_dev_001-edit_BitCraze_DD
        return partition->offset;
    else
        return UINT32_MAX;
}
<<<<<<< HEAD

void pi_partition_table_free(pi_partition_table_t table)
{
    flash_partition_table_free((flash_partition_table_t *) table);
}

pi_err_t pi_partition_table_load(pi_device_t *flash, const pi_partition_table_t *table)
{
    pi_err_t rc;
    
    rc = flash_partition_table_load(flash, (const flash_partition_table_t **) table, NULL);
//    if (rc == PI_OK)
//    {
//    flash_partition_print_partition_table((flash_partition_table_t *) *table);
//    }
    
    return rc;
}

const pi_partition_t *
pi_partition_find_first(const pi_partition_table_t table, const pi_partition_type_t type,
                        const pi_partition_subtype_t subtype,
                        const char *label)
{
    pi_partition_t *partition;
    const flash_partition_info_t *info;
    
    info = flash_partition_find_first((const flash_partition_table_t *) table, type, subtype, label);
    if(info == NULL)
        return NULL;
    
    partition = pi_l2_malloc(sizeof(pi_partition_t));
    partition->type = info->type;
    partition->subtype = info->subtype;
    partition->size = info->pos.size;
    partition->offset = info->pos.offset;
    memcpy(partition->label, info->label, 16);
    partition->label[16] = 0;
    partition->encrypted = false;
    partition->read_only = false;
    
    return (const pi_partition_t *) partition;
}

=======
>>>>>>> 3.1.1_dev_001-edit_BitCraze_DD
