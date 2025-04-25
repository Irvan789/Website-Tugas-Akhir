<script setup lang="ts">
import moment from "moment"

const props = defineProps<{
  time: number
  format: string
}>()

const result = ref<string>("")

onMounted(() => {
  result.value = formatUnixTime(props.time, props.format)
})

function formatUnixTime(time: number, format: string) {
  return moment.unix(time).format(format)
}
</script>

<template>
  {{ result }}
</template>
