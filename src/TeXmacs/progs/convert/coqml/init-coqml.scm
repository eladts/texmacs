
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : init-coqml.scm
;; DESCRIPTION : Setup CoqMl converters
;; COPYRIGHT   : (C) 2013  François Poulain, Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (convert coqml init-coqml))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CoqMl
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;(define-format coqml
;  (:name "")
;  (:suffix ""))

(lazy-define (convert coqml coqmltm)     parse-coqml-snippet)
(lazy-define (convert coqml coqmltm)     parse-coqml-document)
(lazy-define (convert coqml coqmltm)     coqml->texmacs)
(lazy-define (convert coqml tmcoqml)     texmacs->coqml)
(lazy-define (convert coqml coqtopmlout) serialize-coqml)

(converter coqml-document coqml-stree
  (:function parse-coqml-document))

(converter coqml-stree coqml-document
  (:function serialize-coqml))

(converter coqml-snippet coqml-stree
  (:function parse-coqml-snippet))

(converter coqml-stree coqml-snippet
  (:function serialize-coqml))

;(converter coqml-stree texmacs-stree
;  (:function coqml->texmacs))
;
;(converter texmacs-stree coqml-stree
;  (:function texmacs->coqml))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Vernacular
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define-format vernac
  (:name "Coq Vernacular")
  (:suffix "v"))

(lazy-define (convert coqml tmvernac) texmacs->vernac)
(lazy-define (convert coqml tmvernac) texmacs->vernac-document)

(converter vernac-snippet texmacs-tree
  (:function vernac->texmacs))

(converter vernac-document texmacs-tree
  (:function vernac-document->texmacs))

(converter texmacs-tree vernac-snippet
  (:function texmacs->vernac))

(converter texmacs-tree vernac-document
  (:function texmacs->vernac-document))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CoqTopML
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(lazy-define (convert coqml coqtopmlout) serialize-coqtopml)
(lazy-define (convert coqml scmcoqtopml) stree->coqtopml)
(lazy-define (convert coqml coqtopmlscm) coqtopml->stree)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Tests
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(lazy-define (convert coqml coqtopmltest) test-coqtopml)
