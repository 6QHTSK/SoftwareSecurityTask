from Description.Regedit.RegCloseKey import description as regCloseKey
from Description.Regedit.RegCreateKeyExA import description as regCreateKeyExA
from Description.Regedit.RegOpenKeyExA import description as regOpenKeyExA
from Description.Regedit.RegQueryValueExA import description as regQueryValueExA
from Description.Regedit.RegSetValueExA import description as regSetValueExA

def regEditDescription(event):
  operation = event["event"]["Operation"]
  if operation == "RegCloseKey":
    regCloseKey(event)
  elif operation == "RegCreateKeyExA":
    regCreateKeyExA(event)
  elif operation == "RegOpenKeyExA":
    regOpenKeyExA(event)
  elif operation == "RegQueryValueExA":
    regQueryValueExA(event)
  elif operation == "RegSetValueExA":
    regSetValueExA(event)