# -*- coding: utf-8 -*-

import asyncio
import platform
import clr
import sys
import pathlib
from typing import Callable, Any, Union
from asyncio.events import AbstractEventLoop
from functools import wraps
from asyncio.tasks import ensure_future
import logging

logger = logging.getLogger("asyncio")

_here = pathlib.Path(__file__).parent
sys.path.append(str(pathlib.Path(__file__).parent))


clr.AddReference("BleakUWPBridge")
"""
clr.AddReference("Windows.Foundation")
clr.AddReference("Windows.Devices.Bluetooth")
clr.AddReference("Windows.Storage.Streams")
clr.AddReference("Windows.Devices.Bluetooth.GenericAttributeProfile")
"""
from BleakBridge import Bridge

import System

from Windows.Foundation import (
IAsyncOperation, 
AsyncOperationCompletedHandler,
TypedEventHandler,
AsyncStatus
)

from Windows.Devices.Bluetooth import(
BluetoothError,
#ByteOrder
)

from Windows.Storage.Streams import(
DataReader
)

from Windows.Devices.Bluetooth.GenericAttributeProfile import (
GattServiceProviderResult,
GattServiceProvider,
GattLocalCharacteristicResult,
GattLocalCharacteristicParameters,
GattCharacteristicProperties,
GattProtectionLevel,
GattLocalCharacteristic,
GattWriteRequest,
GattServiceProviderAdvertisingParameters,
GattPresentationFormat,
GattServiceProviderAdvertisementStatusChangedEventArgs,
GattWriteRequestedEventArgs
)

class BleakError(Exception):
    """Base Exception for bleak."""

    pass


class BleakDotNetTaskError(BleakError):
    """Wrapped exception that occurred in .NET async Task."""

    pass

def enable_logging():
        l = logging.getLogger("asyncio")
        l.setLevel(logging.DEBUG)
        h = logging.StreamHandler(sys.stdout)
        h.setLevel(logging.DEBUG)
        l.addHandler(h)
        logger.addHandler(h)

async def wrap_IAsyncOperation(op, return_type, loop):
    """Enables await on .NET Task using asyncio.Event and a lambda callback.

    Args:
        task (System.Threading.Tasks.Task): .NET async task object to await.
        loop (Event Loop): The event loop to await on the Task in.

    Returns:
        The results of the the .NET Task.

    """
    done = asyncio.Event(loop=loop)
    # Register AsyncOperationCompletedHandler callback that triggers the above asyncio.Event.
    op.Completed = AsyncOperationCompletedHandler[return_type](
        lambda x, y: loop.call_soon_threadsafe(done.set)
    )
    # Wait for callback.
    await done.wait()

    if op.Status == AsyncStatus.Completed:
        return op.GetResults()
    elif op.Status == AsyncStatus.Error:
        # Exception occurred. Wrap it in BleakDotNetTaskError
        # to make it catchable.
        raise BleakDotNetTaskError(op.ErrorCode.ToString())
    else:
        # TODO: Handle IsCancelled.
        raise BleakDotNetTaskError("IAsyncOperation Status: {0}".format(op.Status))

def ProcessWriteCharacteristic(request, opCode):
    if (request.Value.Length !=4 ):
        print ("Error with length in request!")
        exit()
    else:
        reader = DataReader.FromBuffer(request.Value)
        #reader.ByteOrder = ByteOrder.LittleEndian
        val = reader.ReadInt32()
        print (val)


async def ServiceProviderInitAsync(service_uuid, char_uuid, loop):
    serviceResult = await wrap_IAsyncOperation(
            IAsyncOperation[GattServiceProviderResult](
                GattServiceProvider.CreateAsync(service_uuid)
            ),
            return_type=GattServiceProviderResult,
            loop=loop,
        )
    if (serviceResult.Error == BluetoothError.Success):
            serviceProvider = serviceResult.ServiceProvider
            print ("Service success")
    else:
            print ("Service couldn't be started")
            exit()
            
    char_parameters = GattLocalCharacteristicParameters(
    CharacteristicProperties = GattCharacteristicProperties.Write or GattCharacteristicProperties.WriteWithoutResponse,
    WriteProtectionLevel = GattProtectionLevel.Plain,
    UserDescripton = "mouseCharacteristic")
    
    result = await wrap_IAsyncOperation(
            IAsyncOperation[GattLocalCharacteristicResult](
                serviceProvider.Service.CreateCharacteristicAsync(char_uuid, char_parameters)
            ),
            return_type=GattLocalCharacteristicResult,
            loop=loop,
        )
    if (result.Error == BluetoothError.Success):
        opCharacteristic = result.Characteristic
        print ("Characteristic success")
    else:
        print ("Problem with starting Characteristic")
        exit()
    
    def WriteRequestedAsync(sender, args):
        logger.debug("A write is requested!")
    
    def ServiceProvider_AdvertisementStatusChanged(sender, args):
        logger.debug("Advertisement status changed!")
        
    opCharacteristic.WriteRequested += WriteRequestedAsync
    serviceProvider.AdvertisementStatusChanged += ServiceProvider_AdvertisementStatusChanged
    
    advertising_parameters = GattServiceProviderAdvertisingParameters(
    IsConnectable = System.Convert.ToBoolean(1),
    IsDiscoverable =System.Convert.ToBoolean(1)
    )
    
    serviceProvider.StartAdvertising(advertising_parameters)
    logger.debug("started adv")
    await asyncio.sleep(60, loop=loop)
    serviceProvider.StopAdvertising()
    await asyncio.sleep(5)
    
        
if __name__ == "__main__":
    service_uuid = System.Guid.Parse("caecface-e1d9-11e6-bf01-fe55135034f0")
    char_uuid = System.Guid.Parse("caec2ebc-e1d9-11e6-bf01-fe55135034f1")
    enable_logging()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(ensure_future(ServiceProviderInitAsync(service_uuid, char_uuid, loop)))