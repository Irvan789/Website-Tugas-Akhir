<script setup lang="ts">
import type { ApexOptions } from "apexcharts"

const props = defineProps<{
  height: string
  series: ApexAxisChartSeries
  options?: ApexOptions
}>()

const chartOptions: ApexOptions = {
  chart: {
    type: "area",
    fontFamily: "Signika Negative",
    animations: {
      enabled: false
    },
    toolbar: {
      show: false
    },
    zoom: {
      enabled: false
    }
  },
  dataLabels: {
    enabled: false
  },
  ...props.options
}
</script>

<template>
  <apexchart :height="props.height" :options="chartOptions" :series="props.series" />
</template>
