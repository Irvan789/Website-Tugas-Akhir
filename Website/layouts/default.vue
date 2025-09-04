<script setup lang="ts">
import { OverlayScrollbarsComponent } from "overlayscrollbars-vue"
import "overlayscrollbars/styles/overlayscrollbars.css"

const route = useRoute()
const modalConfig = ref<boolean>(false)
</script>

<template>
  <div
    class="relative flex h-full w-full flex-col"
    :class="[modalConfig ? 'overflow-hidden' : 'overflow-x-hidden overflow-y-auto']"
  >
    <div class="sticky top-0 z-[10] w-full bg-[#294899] text-white">
      <div class="mx-auto flex max-w-7xl flex-row items-center justify-between gap-x-4 px-3 py-2.5">
        <NuxtLink class="text-xl" href="/">CrunchyNuts IoT</NuxtLink>
        <div class="flex flex-row items-center gap-x-3">
          <NuxtLink
            :class="{
              'text-gray-200 transition-colors duration-300 hover:text-white':
                route.path != '/history'
            }"
            href="/history"
          >
            Histori
          </NuxtLink>
          <button
            class="cursor-pointer text-base text-gray-200 transition-colors duration-300 hover:text-white"
            @click="modalConfig = !modalConfig"
          >
            Konfigurasi
          </button>
        </div>
      </div>
    </div>
    <OverlayScrollbarsComponent :options="{ scrollbars: { visibility: 'hidden' } }" defer>
      <div class="relative h-full max-h-[calc(100%-52px)] w-full text-black">
        <div class="mx-auto grid w-full max-w-7xl grid-flow-row gap-y-2.5 px-3 py-3">
          <slot />
        </div>
        <div
          class="mx-auto flex max-w-7xl flex-col px-3 pb-2.5 text-center text-sm leading-5 text-[#20529d]/50"
        >
          CrunchyNuts IoT | Irvan Tonny Widjaya (119400018)
          <div class="flex justify-center text-xs">
            Made With Tailwind, ApexChart, Firebase, & Nuxt
          </div>
        </div>
      </div>
    </OverlayScrollbarsComponent>
  </div>
  <ModalConfig :display="modalConfig" @close="modalConfig = false" />
</template>
