void sched_block(uint32_t pid)
{
    processes[pid].state_flags = SCHED_STATE_BLOCKED;
}

void sched_unblock(uint32_t pid)
{
    processes[pid].state_flags = SCHED_STATE_READY;
}