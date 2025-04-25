<script setup lang="ts">
import moment from "moment"
import type { IStatistics } from "~/types/database"

const props = defineProps<{
  statistics: Array<IStatistics>
}>()

function formatTime(time: number) {
  return moment.unix(time).format("ss")
}
</script>

<template>
  <div class="grid grid-cols-1 gap-2.5 lg:grid-cols-3">
    <CardData
      unit="V"
      name="Tegangan PLN"
      icon="mingcute:lightning-line"
      :value="props.statistics.at(-1)?.vpln.toFixed(0) ?? 0"
    />
    <CardData
      unit="V"
      name="Tegangan PLTS"
      icon="mingcute:solar-panel-line"
      :value="props.statistics.at(-1)?.vplts.toFixed(0) ?? 0"
    />
    <CardData
      unit="V"
      name="Tegangan Baterai"
      icon="mingcute:battery-automotive-line"
      :value="props.statistics.at(-1)?.vbatt.toFixed(2) ?? (0).toFixed(2)"
    />
  </div>
  <div class="grid grid-cols-1 gap-2.5 lg:grid-cols-2">
    <ClientOnly>
      <CardArea>
        <ApexChart
          height="300"
          :options="chartVPLNOptions"
          :series="[
            {
              name: 'Tegangan PLN',
              data: props.statistics.map((v) => ({ x: formatTime(v.time), y: v.vpln }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="300"
          :options="chartVPLTSOptions"
          :series="[
            {
              name: 'Tegangan PLTS',
              data: props.statistics.map((v) => ({ x: formatTime(v.time), y: v.vplts }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="300"
          :options="chartLPLNOptions"
          :series="[
            {
              name: 'Penggunaan Daya PLN',
              data: props.statistics.map((v) => ({ x: formatTime(v.time), y: v.lpln }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="300"
          :options="chartLPLTSOptions"
          :series="[
            {
              name: 'Penggunaan Daya PLTS',
              data: props.statistics.map((v) => ({ x: formatTime(v.time), y: v.lplts }))
            }
          ]"
        />
      </CardArea>
      <template #fallback>
        <div
          class="col-span-2 flex h-[318px] w-full items-center justify-center bg-sky-200/20 px-4 py-5 shadow shadow-sky-200/40 lg:h-[508px]"
        >
          Loading Chart Data
        </div>
      </template>
    </ClientOnly>
  </div>
</template>
