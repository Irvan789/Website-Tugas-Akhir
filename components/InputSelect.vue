<script setup lang="ts">
const props = defineProps<{
  name: string
  value: string[]
  selected?: string
  disabled?: boolean
}>()

const emits = defineEmits<{
  change: [v: string]
}>()
</script>

<template>
  <div class="flex flex-row items-center justify-between text-sm">
    {{ props.name }}
    <div class="flex gap-x-2">
      <select
        class="w-[83px] rounded border border-gray-200 bg-white px-0.5 py-0.5 focus:outline-none disabled:cursor-not-allowed disabled:opacity-80"
        :disabled="props.disabled"
        @change="(e) => emits('change', (e.currentTarget as HTMLInputElement).value)"
      >
        <template v-for="v in props.value">
          <option :selected="props.selected == v">{{ v }}</option>
        </template>
      </select>
    </div>
  </div>
</template>
