<template>
  <div>
    <h1>Hello from Vue</h1>
    <button type="button" @click="b1()">B1</button>
    <button type="button" @click="b2()">B2</button>
    <pre>{{ result }} </pre>
  </div>
</template>

<script>
export default {
  data: () => ({
    result: ''
  }),

  methods: {
    b1(){
      this.fetch('/b1')
    },

    b2(){
      this.fetch('/b2')
    },

    async fetch(url){
      const host = document.location.hostname === 'localhost' ? 'http://192.168.1.201' : ''

      try{
        const res = await fetch(`${host}${url}`)
        this.result = await res.json()
      } catch(e){
        console.error('🔥 Fetch 🔥', err)
      }
    }

  }
}
</script>