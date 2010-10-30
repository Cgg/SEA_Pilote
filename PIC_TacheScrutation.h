/* Pilote de capteur :
 * 
 * Description de la tâche de scrutation des messages de capteur.
 * 
 * Le role de cette tache est de scruter les messages en provenance des capteurs
 * physiques. Lorsqu'un message arrive, il declenche une interruption. Le 
 * handler d'it place le message dans un buffer intermediaire ou cette tache 
 * vient le recuperer. Elle attache un numero de message et un timestamp au 
 * message et le place dans la boite au lettre correspondante au capteur.
 *
 * copyright 2010 Lize / Geiger
 */
