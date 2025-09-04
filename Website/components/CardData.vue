<script setup lang="ts">
import { Icon } from "@iconify/vue"

const props = defineProps<{
  name: string
  value: string | number
  class?: string
  unit: string
  icon: string
}>()
</script>

<template>
  <div
    class="flex flex-row items-center gap-x-2.5 rounded-md bg-sky-200/25 px-3 py-2.5 shadow shadow-sky-200/40"
    :class="props.class"
  >
    <div class="flex items-center justify-center rounded-md bg-blue-500 p-1.5 text-white">
      <Icon :icon="props.icon" class="text-xl" />
    </div>
    <div class="mt-0.5 flex flex-col text-base leading-4 font-semibold shadow-sky-800">
      {{ props.name }}
      <div class="leading-2 font-medium">
        {{ props.value }}<span class="text-sm">{{ props.unit }}</span>
      </div>
    </div>
  </div>
</template>
