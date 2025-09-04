<script setup lang="ts">
const props = defineProps<{
  class?: string
  type: "submit" | "button"
  disabled?: boolean
}>()

const emits = defineEmits<{
  click: []
}>()

const componentRef = useTemplateRef("ref")

defineExpose({
  componentRef
})
</script>

<template>
  <button
    class="cursor-pointer rounded bg-[#20529d]/90 text-white transition-colors duration-300 hover:bg-[#20529d] disabled:cursor-not-allowed disabled:bg-[#2688f0]"
    :class="[props.class ?? '']"
    :type="props.type"
    :disabled="props.disabled ?? false"
    @click="emits('click')"
    ref="ref"
  >
    <slot />
  </button>
</template>
