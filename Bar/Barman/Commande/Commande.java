import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;
import java.io.*;
import java.net.*;

public class Commande extends UnicastRemoteObject {
    Commande() throws RemoteException{
    }
    final static int port = 1909;
    final static int taille = 1024;
    static byte buffer[] = new byte[taille];
    static byte bufferIn[] = new byte[taille];
    static boolean running = true;
    static byte[] color;
    public static void main(String[] args) throws IOException{
        Scanner scanner = new Scanner(System.in);

        try{
            DatagramSocket socket = new DatagramSocket(port);
            DatagramPacket message = new DatagramPacket(bufferIn, buffer.length);
            System.out.println("Démarrage boucle");
            //démarrage IHM
            while (running){

            socket.receive(message);
            InetAddress clientAddress = message.getAddress();
            int clientPort = message.getPort();
            int typeBiere = Integer.parseInt(new String(message.getData(), 0, message.getLength()));
                try {
                    IBiere stub = (IBiere)Naming.lookup("rmi://localhost:2020/Fournisseur");
                    switch(typeBiere){
                        case 1:
                            System.out.println("Voici la liste de nos bières Ambrées:");
                            for (Biere bir : stub.listeAmbrees()) {
                                System.out.println(bir.getNom());
                            }

                            break;

                        case 2:
                            System.out.println("Voici la liste de nos bières Blondes");
                            for (Biere bir : stub.listeBlondes()) {
                                System.out.println(bir.getNom());
                            }
                            break;
                        default:
                            System.out.println("Requete Invalide");
                    }
                    String nextRequete = scanner.nextLine();
                    Biere bir2 = stub.acheterBiere(nextRequete);

                    while (bir2 == null){
                        System.out.println("La bière en question n'est pas disponible, veuillez choisir une autre");
                        nextRequete = scanner.nextLine();
                        bir2 = stub.acheterBiere(nextRequete); 
                    }
                    buffer = nextRequete.getBytes();
                    DatagramPacket response = new DatagramPacket(buffer, buffer.length, clientAddress, clientPort);
                    socket.send(response);
                }
                catch (Exception ae){
                    System.out.println(ae);
                    running = false;
                }
            }
        }
        catch (SocketException se){
            System.out.println(se);
        }



    }
}
