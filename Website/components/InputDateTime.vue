<script setup lang="ts">
const props = defineProps<{
  value?: string
  disabled?: boolean
}>()

const emits = defineEmits<{
  change: [v: string]
}>()
</script>

<template>
  <div class="flex flex-col justify-between text-sm">
    <input
      type="datetime-local"
      class="rounded border border-gray-200 bg-white px-2 py-1 focus:outline-none disabled:cursor-not-allowed disabled:bg-white disabled:opacity-80"
      :value="props.value"
      :disabled="props.disabled ?? false"
      @change="(e) => emits('change', (e.currentTarget as HTMLInputElement).value)"
    />
  </div>
</template>
