#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>

static int pgstats_show(struct seq_file *m, void *v) {
    struct task_struct *task;

    for_each_process(task) {
        seq_printf(m, "%d: [[%lu],[%lu],[%lu]], [[%lu],[%lu],[%lu]], [[%lu],[%lu],[%lu]], [[%lu],[%lu],[%lu]]\n",
                   task->pid,
                   task->pgd_alloc, task->pgd_free, task->pgd_set,
                   task->pud_alloc, task->pud_free, task->pud_set,
                   task->pmd_alloc, task->pmd_free, task->pmd_set,
                   task->pte_alloc, task->pte_free, task->pte_set);
    }
    return 0;
}

static int pgstats_open(struct inode *inode, struct file *file) {
    return single_open(file, pgstats_show, NULL);
}

static const struct proc_ops pgstats_fops = {
    .proc_open = pgstats_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init pgstats_init(void) {
    proc_create("pg_stats", 0, NULL, &pgstats_fops);
    return 0;
}

static void __exit pgstats_exit(void) {
    remove_proc_entry("pg_stats", NULL);
}

module_init(pgstats_init);
module_exit(pgstats_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Page Table Statistics Module");
