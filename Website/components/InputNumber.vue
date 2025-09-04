<script setup lang="ts">
const props = defineProps<{
  name: string
  value: number
  step?: number
  unit?: string
  disabled?: boolean
}>()

const emits = defineEmits<{
  change: [v: number]
}>()
</script>

<template>
  <div class="flex flex-row items-center justify-between text-sm">
    {{ props.name }}
    <div class="flex items-center gap-x-1.5">
      <input
        min="0"
        type="number"
        class="w-16 rounded border border-gray-200 bg-white py-0.25 pl-1.5 focus:outline-none disabled:cursor-not-allowed disabled:bg-white disabled:opacity-80"
        :step="props.step ?? 1"
        :value="props.value"
        :disabled="props.disabled ?? false"
        @change="(e) => emits('change', Number((e.currentTarget as HTMLInputElement).value))"
      />
      <div class="text-right">{{ props.unit }}</div>
    </div>
  </div>
</template>
