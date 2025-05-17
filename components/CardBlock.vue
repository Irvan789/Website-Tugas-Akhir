<script setup lang="ts">
const props = defineProps<{
  class?: string
}>()
</script>

<template>
  <div
    class="flex w-full flex-col gap-2.5 rounded-md bg-sky-200/25 shadow shadow-sky-200/40"
    :class="props.class"
  >
    <slot />
  </div>
</template>
