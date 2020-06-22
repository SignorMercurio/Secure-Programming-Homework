import Vue from 'vue'

Vue.prototype.$success = function(ops, icon = 'check_circle') {
  this.$q.notify({
    type: 'positive',
    icon: icon,
    message: `${ops} successfully`
  })
}

Vue.prototype.$fail = function(msg) {
  this.$q.notify({
    type: 'negative',
    message: msg
  })
}

Vue.prototype.$now = function() {
  return new Date().toLocaleTimeString('zh-cn', { hour12: false })
}
