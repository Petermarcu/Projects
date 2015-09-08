sudo rm /home/pi/RFHub/HubMasterLog.1
sudo mv /home/pi/RFHub/HubMasterLog.txt ./HubMasterLog.1
sudo rm /home/pi/RFHub/HubMasterStderr.1
sudo mv /home/pi/RFHub/HubMasterStderr.txt ./HubMasterStderr.1
sudo /etc/init.d/StartHubMaster.sh start 
