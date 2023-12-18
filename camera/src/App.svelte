<script lang="ts">
    let canvas: HTMLCanvasElement | null = null
    let video: HTMLVideoElement | null = null
    let ctx: CanvasRenderingContext2D | null = null
    let message = $state('')

    const interval = setInterval(() => {
        fetch('/photo').then((res) => {
            if (res.status == 302) {
                takePhoto()
                clearInterval(interval)
            }
        })
    }, 1000)

    function displayOnCanvas() {
        if (ctx) {
            ctx.drawImage(video!, 0, 0, canvas!.width, canvas!.height)
        }

        requestAnimationFrame(displayOnCanvas)
    }

    $effect(() => {
        navigator.mediaDevices
            .getUserMedia({
                video: {
                    facingMode: 'environment',
                },
            })
            .then((stream) => {
                video!.srcObject = stream

                video!.addEventListener('loadedmetadata', () => {
                    const aspectRatio = video!.videoWidth / video!.videoHeight
                    canvas!.width = window.innerWidth
                    canvas!.height = window.innerWidth / aspectRatio
                })

                ctx = canvas!.getContext('2d')!

                video!.addEventListener('play', () => {
                    displayOnCanvas()
                })
            })
            .catch((error) => {
                console.error('Error accessing camera:', error)
            })
    })

    async function takePhoto() {
        message = 'Taking photo...'
        if (ctx) {
            ctx.drawImage(video!, 0, 0, canvas!.width, canvas!.height)

            // Convert the canvas content to a data URL representing the image
            const dataURL = canvas!.toDataURL('image/png')

            // Extract the base64-encoded string from the data URL
            const base64String = dataURL.split(',')[1].trim()

            // Send the image data to the server using fetch
            const response = await fetch('/upload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ image: base64String }),
            })

            // Handle the server response as needed
            const result = await response.json()
            message = result.message
        }
    }
</script>

<div class="h-screen flex flex-col justify-center items-center">
    <video bind:this={video} autoplay class="hidden">
        <track kind="captions" />
    </video>
    <canvas bind:this={canvas} />
    {message}
</div>
