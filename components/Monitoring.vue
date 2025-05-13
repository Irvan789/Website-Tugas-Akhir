<script setup lang="ts">
import type { IStatistics } from "~/types/database"

const props = defineProps<{
  statistics: Array<IStatistics>
}>()
</script>

<template>
  <div class="grid grid-cols-2 gap-2.5 md:grid-cols-3">
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
      class="col-span-2 md:col-span-1"
      :value="props.statistics.at(-1)?.vbatt.toFixed(2) ?? (0).toFixed(2)"
    />
  </div>
  <div class="grid grid-cols-2 gap-2.5 lg:grid-cols-6">
    <CardData
      unit="A"
      name="Arus Relay 1"
      icon="mdi:current-ac"
      :value="props.statistics.at(-1)?.relay.n1.current.toFixed(3) ?? 0.0"
    />
    <CardData
      unit="W"
      name="Power Relay 1"
      icon="solar:lightbulb-bolt-linear"
      :value="props.statistics.at(-1)?.relay.n1.power.toFixed(0) ?? 0"
    /><CardData
      unit="A"
      name="Arus Relay 2"
      icon="mdi:current-ac"
      :value="props.statistics.at(-1)?.relay.n2.current.toFixed(3) ?? 0.0"
    />
    <CardData
      unit="W"
      name="Power Relay 2"
      icon="solar:lightbulb-bolt-linear"
      :value="props.statistics.at(-1)?.relay.n2.power.toFixed(0) ?? 0"
    /><CardData
      unit="A"
      name="Arus Relay 3"
      icon="mdi:current-ac"
      :value="props.statistics.at(-1)?.relay.n3.current.toFixed(3) ?? 0.0"
    />
    <CardData
      unit="W"
      name="Power Relay 3"
      icon="solar:lightbulb-bolt-linear"
      :value="props.statistics.at(-1)?.relay.n3.power.toFixed(0) ?? 0"
    />
  </div>
  <div class="grid grid-cols-1 gap-2.5 lg:grid-cols-2">
    <ClientOnly>
      <CardArea>
        <ApexChart
          height="280"
          :options="chartVPLNOptions"
          :series="[
            {
              name: 'Tegangan PLN',
              data: props.statistics.map((v) => ({ x: v.time.toString(), y: v.vpln }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="280"
          :options="chartVPLTSOptions"
          :series="[
            {
              name: 'Tegangan PLTS',
              data: props.statistics.map((v) => ({ x: v.time.toString(), y: v.vplts }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="280"
          :options="chartLPLNOptions"
          :series="[
            {
              name: 'Penggunaan Daya PLN',
              data: props.statistics.map((v) => ({ x: v.time.toString(), y: v.lpln }))
            }
          ]"
        />
      </CardArea>
      <CardArea>
        <ApexChart
          height="280"
          :options="chartLPLTSOptions"
          :series="[
            {
              name: 'Penggunaan Daya PLTS',
              data: props.statistics.map((v) => ({ x: v.time.toString(), y: v.lplts }))
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
