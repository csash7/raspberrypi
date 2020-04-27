from kafka import KafkaProducer

producer = KafkaProducer(bootstrap_servers=['iotproject.space:9092'], api_version=(2,4,0))

# Asynchronous by default
future = producer.send('test', value=b'off').get(timeout=30)