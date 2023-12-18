import { createRoot } from 'svelte';
import App from './App.svelte'
import './app.css'

const target: any = document.getElementById("app");

const app = createRoot(App, { target: target});

export default app
