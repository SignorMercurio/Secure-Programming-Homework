import Vue from 'vue'
import axios from 'axios'
import { flask_url } from 'src/statics/config'

Vue.prototype.$axios = axios.create({
  baseURL: flask_url,
  timeout: 40000
})

Vue.prototype.$token = () => {
  let state = JSON.parse(sessionStorage.getItem('store'))
  if (state) {
    let info = state.userInfo
    if (info) {
      return 'Basic ' + window.btoa(info.token + ':')
    }
    return ''
  }
}

Vue.prototype.$axios.interceptors.request.use(
  config => {
    config.headers.Authorization = Vue.prototype.$token()
    return config
  },
  err => {
    Promise.reject(err)
  }
)

Vue.prototype.$axios.interceptors.response.use(
  res => {
    if (res.data.code && res.data.code !== 0) {
      Vue.prototype.$q.notify({
        type: 'negative',
        message: res.data.msg
      })
    } else return res
  },
  err => {
    Vue.prototype.$q.notify({
      type: 'negative',
      message: 'Request timeout...'
    })
    Promise.reject(err)
  }
)
