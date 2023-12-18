import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'
import mkcert from'vite-plugin-mkcert'



// https://vitejs.dev/config/
export default defineConfig({
    plugins: [svelte(), mkcert()],
    server: {
        https: true,
        proxy: {
            '/photo': {
                target: 'http://localhost:8000',
            },
            '/upload': {
                target: 'http://localhost:8000',

            }
        },
    },
})
