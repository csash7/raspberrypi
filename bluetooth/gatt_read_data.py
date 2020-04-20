import asyncio
from bleak import BleakClient

address = "db:51:e9:77:c8:25"
MODEL_NBR_UUID = "19B10010-E8F2-537E-4F6C-D104768A1214"

async def run(address, loop):
    client = BleakClient(address, loop=loop)
    try:
        await client.connect()
        model_number = await client.read_gatt_char(MODEL_NBR_UUID)
        print("Model Number: {0}".format("".join(map(chr, model_number))))
    except Exception as e:
        print(e)
    finally:
        await client.disconnect()

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address, loop))
