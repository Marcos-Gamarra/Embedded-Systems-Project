<script lang="ts">
    interface Props {
        prop1: number;
        prop2: number;
        prop3: number;
    }
    let { prop1, prop2, prop3 } = $props<Props>();
</script>

<h1>{prop1}</h1>
<h1>{prop2}</h1>
<h1>{prop3}</h1>
