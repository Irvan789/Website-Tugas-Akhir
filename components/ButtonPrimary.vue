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
    class="cursor-pointer rounded px-3 py-2 transition-colors duration-300 disabled:cursor-not-allowed"
    :class="[props.class ?? '']"
    :type="props.type"
    :disabled="props.disabled ?? false"
    @click="emits('click')"
    ref="ref"
  >
    <slot />
  </button>
</template>
