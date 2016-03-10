from jsonschema import validate
import json

with open('preset-schema.json') as schema_file, open('presets.json') as data_file:
    schema = json.load(schema_file)
    print("parsing presets.json... ", end="")
    data = json.load(data_file)
    print("presets.json successfully parsed!")

    print("validating against schema... ", end="")
    validate(data, schema)
    print("schema determined valid!")

    print("now tring to upload to server")

    import pysftp
    import sys

    with pysftp.Connection('roborio-973-frc.local', username='lvuser', password='') as sftp:
        sftp.put('presets.json')

    print("finished uploading to server")

print("finished")
