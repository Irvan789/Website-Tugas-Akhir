<script setup lang="ts">
import { Icon } from "@iconify/vue"

const props = defineProps<{
  name: string
  value: string | number
  unit: string
  icon: string
}>()
</script>

<template>
  <div
    class="flex flex-row items-center gap-x-3 rounded-md bg-sky-200/25 px-4 py-5 shadow shadow-sky-200/40"
  >
    <div class="flex items-center justify-center rounded-md bg-blue-500 p-1.5 text-white">
      <Icon :icon="props.icon" class="text-xl" />
    </div>
    <div class="flex flex-col text-lg font-semibold leading-6 shadow-sky-800">
      {{ props.name }}
      <div class="font-medium leading-5">
        {{ props.value }}<span class="text-sm font-medium">{{ props.unit }}</span>
      </div>
    </div>
  </div>
</template>
