import keras_ocr
import cv2
import asyncio
from aiohttp import web
import base64

connected_client = set()


# this function is used to get the action from the image (left or right)
def get_action():
    image = cv2.imread("action.jpg")

    # use keras-ocr to detect the words
    pipeline = keras_ocr.pipeline.Pipeline()
    prediction_groups = pipeline.recognize([image])

    # print the words
    for words in prediction_groups[0]:
        print("\n" + words[0].strip() + "\n")
        return words[0].strip()


# this variable is used to check if the photo should be taken
should_take_photo = False


# this function is used to handle the object detection request
async def handle_object_detection(request):
    global should_take_photo
    should_take_photo = True
    await asyncio.sleep(3)
    action = get_action()
    print(f"Action: {action}")
    return web.Response(text=action)


# this function is used to handle the photo request
async def handle_photo(request):
    global should_take_photo
    if should_take_photo:
        should_take_photo = False
        return web.Response(status=302)
    else:
        return web.Response(status=200)


# this function is used to handle the image upload request
async def handle_upload(request):
    print("Handling upload")
    try:
        data = await request.json()
        image_data = data.get('image', '')

        # Decode base64-encoded image data
        image_bytes = base64.b64decode(image_data)

        # You can now process the image data as needed
        # For example, save it to a file or perform further analysis
        with open('action.jpg', 'wb') as f:
            f.write(image_bytes)

        # Respond with the result or any other appropriate response
        return web.Response(status=200)
    except Exception as e:
        print(f"Error handling upload: {e}")
        return web.Response(status=500)


async def main():
    # WebSocket server

    # HTTP server
    app = web.Application()
    app.router.add_route('GET', '/object', handle_object_detection)
    app.router.add_route('GET', '/photo', handle_photo)
    app.router.add_route('POST', '/upload',
                         handle_upload)  # Added route for image upload

    # Create a TCP server with the app
    http_server = await loop.create_server(app.make_handler(), "0.0.0.0", 8000)

    await asyncio.gather(http_server.serve_forever())


if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    loop.run_forever()
