<script setup lang="ts">
import type { NuxtError } from "#app"

const props = defineProps({
  error: Object as () => NuxtError
})

useSeoMeta({
  title: `${props.error?.statusCode} | Tugas Akhir | Irvan Tonny Widjaya (119400018)`
})

const dev = import.meta.dev
</script>

<template>
  <div class="font-sourcesans3 flex h-full w-full overflow-hidden font-medium">
    <div class="m-auto flex w-full max-w-4xl flex-col gap-x-4 gap-y-2.5 px-5 py-4">
      <div class="flex justify-between gap-x-4">
        <div class="flex flex-col justify-center">
          <div class="text-4xl">{{ error?.statusCode }}</div>
          <div class="text-lg leading-5">{{ error?.message }}</div>
        </div>
      </div>
      <div v-if="dev && error?.stack" class="flex flex-col gap-1">
        <div class="font-medium">Error Stack:</div>
        <div class="overflow-auto text-[15px]" v-html="error?.stack" />
      </div>
      <NuxtLink
        to="/"
        class="w-fit rounded bg-[#294899]/90 px-3 py-1.5 text-sm text-white transition-colors duration-300 hover:bg-[#294899]"
      >
        Kembali Ke Halaman Utama
      </NuxtLink>
    </div>
  </div>
</template>
