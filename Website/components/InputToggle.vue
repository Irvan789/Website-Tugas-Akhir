<script setup lang="ts">
const props = defineProps<{
  name: string
  value: boolean
  disabled?: boolean
}>()

const emits = defineEmits<{
  change: [v: boolean]
}>()
</script>

<template>
  <div class="flex w-full items-center justify-between gap-x-3 py-0.25 text-sm">
    {{ props.name }}
    <label class="inline-flex cursor-pointer items-center">
      <input
        type="checkbox"
        class="peer sr-only"
        :checked="props.value"
        :disabled="props.disabled ?? false"
        @change="emits('change', !props.value)"
      />
      <div
        class="peer relative h-5 w-10 rounded-full bg-[#3f96f5] peer-checked:bg-[#20529d] peer-focus:outline-none peer-disabled:cursor-not-allowed peer-disabled:opacity-80 after:absolute after:start-1 after:top-1 after:h-3 after:w-3 after:rounded-full after:bg-white after:transition-all after:content-[''] peer-checked:after:translate-x-[170%] peer-checked:after:border-white rtl:peer-checked:after:-translate-x-full"
      ></div>
    </label>
  </div>
</template>
